## FRunnable
`FRunnable`是所有多线程执行的Runnable对象的基类.
```c++
class CORE_API FRunnable
{
public:
	virtual bool Init();
	virtual uint32 Run() = 0;
	virtual void Stop() { }
	virtual void Exit() { }
};
```
这个对象中的这些方法均会新的线程内调用.
其部分子类:
```
FRenderingThread
FRHIThread
FRenderingThreadTickHeartbeat
FTaskThreadBase
FQueuedThread
TAsyncRunnable
FAsyncPurge
```
FRunnable仅仅是定义了期望多线程执行的逻辑, 它需要有真正的线程`(FRunnableThread)`执行它. 上面那些`FRunnable`对象的子类`FRenderingThread`或`FRHIThread`,名字听起来像线程对象, 但他并不是.

## FRunnableThread
这才是线程本体.它提供了一些管理线程对象本身生命周期的接口. FRunnableThread在创建时, 接收一个FRunnable对象实例, 用平台相关的接口创建出真正的线程, 并执行真线程的入口函数, 函数中会执行传入的FRunnable.
```c++
class CORE_API FRunnableThread
{
public:
    // 创建一个线程, 并执行InRunnable, 一切都是从这个函数开始的.
	static FRunnableThread* Create(
		class FRunnable* InRunnable,
		const TCHAR* ThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal,
		uint64 InThreadAffinityMask = FPlatformAffinity::GetNoAffinityMask(),
		EThreadCreateFlags InCreateFlags = EThreadCreateFlags::None);
	virtual void Suspend( bool bShouldPause = true ) = 0;
	virtual bool Kill( bool bShouldWait = true ) = 0;
	virtual void WaitForCompletion() = 0;
protected:
    // 平台相关的创建线程, 并按预期的顺序执行Runnable的逻辑
	virtual bool CreateInternal( FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0, 
		EThreadCreateFlags InCreateFlags = EThreadCreateFlags::None) = 0;
	/** Holds the name of the thread. */
	FString ThreadName;
	/** The runnable object to execute on this thread. */
	FRunnable* Runnable;
	/** Sync event to make sure that Init() has been completed before allowing the main thread to continue. */
	FEvent* ThreadInitSyncEvent;
	/** The priority to run the thread at. */
	EThreadPriority ThreadPriority;
	/** ID set during thread creation. */
	uint32 ThreadID;
private:
	static void SetupCreatedThread(FRunnableThread*& NewThread, class FRunnable* InRunnable, const TCHAR* ThreadName, uint32 InStackSize, EThreadPriority InThreadPri, uint64 InThreadAffinityMask, EThreadCreateFlags InCreateFlags);
};
```
在Windows平台下其实现:
```c++
	virtual bool CreateInternal( FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0,
		EThreadCreateFlags InCreateFlags = EThreadCreateFlags::None) override
	{
		static bool bOnce = false;
		if (!bOnce)
		{
			bOnce = true;
			::SetThreadPriority(::GetCurrentThread(), TranslateThreadPriority(TPri_Normal)); // set the main thread to be normal, since this is no longer the windows default.
		}

		check(InRunnable);
		Runnable = InRunnable;
		ThreadAffinityMask = InThreadAffinityMask;

		// Create a sync event to guarantee the Init() function is called first
		ThreadInitSyncEvent	= FPlatformProcess::GetSynchEventFromPool(true);

		ThreadName = InThreadName ? InThreadName : TEXT("Unnamed UE4");
		ThreadPriority = InThreadPri;

		// Create the new thread
		{
			LLM_SCOPE(ELLMTag::ThreadStack);
			LLM_PLATFORM_SCOPE(ELLMTag::ThreadStackPlatform);
			// add in the thread size, since it's allocated in a black box we can't track
			LLM(FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Default, nullptr, InStackSize));
			LLM(FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Platform, nullptr, InStackSize));

			// 调用Windows API创建线程对象, 入口函数为_ThreadProc
			Thread = CreateThread(NULL, InStackSize, _ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, (::DWORD *)&ThreadID);
		}

		// If it fails, clear all the vars
		if (Thread == NULL)
		{
			Runnable = nullptr;
		}
		else
		{
			ResumeThread(Thread);

			// Let the thread start up
			ThreadInitSyncEvent->Wait(INFINITE);

			ThreadPriority = TPri_Normal; // Set back to default in case any SetThreadPrio() impls compare against current value to reduce syscalls
			SetThreadPriority(InThreadPri);
		}

		// Cleanup the sync event
		FPlatformProcess::ReturnSynchEventToPool(ThreadInitSyncEvent);
		ThreadInitSyncEvent = nullptr;
		return Thread != NULL;
	}
```

其中入口函数: 
```c++
static ::DWORD STDCALL _ThreadProc( LPVOID pThis )
{
	check(pThis);
	auto* ThisThread = (FRunnableThreadWin*)pThis;
	FThreadManager::Get().AddThread(ThisThread->GetThreadID(), ThisThread);
	return ThisThread->GuardedRun();
}
```
pThis为WinAPI创建线程时传入的当前FRunnableThread对象指针.这里将创建的线程添加到`FThreadManager`中管理, 里它实现了在不支持多线程时, 转为单线程执行(Tick FRunnable):
```c++
void FThreadManager::Tick()
{	
	const bool bIsSingleThreadEnvironment = FPlatformProcess::SupportsMultithreading() == false;
	if (bIsSingleThreadEnvironment)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_FSingleThreadManager_Tick);

		FScopeLock ThreadsLock(&ThreadsCritical);

		// Tick all registered fake threads.
		for (TPair<uint32, FRunnableThread*>& ThreadPair : Threads)
		{
			// Only fake and forkable threads are ticked by the ThreadManager
			if( ThreadPair.Value->GetThreadType() != FRunnableThread::ThreadType::Real )
			{
				ThreadPair.Value->Tick();
			}
		}
	}
}
```
 其中`GuardedRun()`z最终调用`Run()`开始执行FRunnable:
```c++
uint32 FRunnableThreadWin::Run()
{
	// Assume we'll fail init
	uint32 ExitCode = 1;
	check(Runnable);

	// Initialize the runnable object
	if (Runnable->Init() == true)
	{
		// Initialization has completed, release the sync event
		ThreadInitSyncEvent->Trigger();

		// Setup TLS for this thread, used by FTlsAutoCleanup objects.
		SetTls();

		// Now run the task that needs to be done
		ExitCode = Runnable->Run();
		// Allow any allocated resources to be cleaned up
		Runnable->Exit();

#if STATS
		FThreadStats::Shutdown();
#endif
		FreeTls();
	}
	else
	{
		// Initialization has failed, release the sync event
		ThreadInitSyncEvent->Trigger();
	}

	return ExitCode;
}
```
这里保证了FRunnable中的接口的执行顺序. 在`CreateInternal()`创建线程之后, 并不是立即返回, 而是等待一个同步事件:
```c++
// Let the thread start up
ThreadInitSyncEvent->Wait(INFINITE);
```
这个事件就是在`Run()`中执行完`Runnable->Init()`之后发出的.这就保证了, `FRunnableThread::Create()`一定是在`FRunnable`初始化完之后才会返回.
除此之外还有许多其它平台的实现:
```
FRunnableThreadPThread
    FRunnableThreadApple
	FRunnableThreadUnix
	FRunnableThreadAndroid
FFakeThread
FForkableThread
FRunnableThreadHoloLens// 混合现实眼镜
```
所以, 这组API的使用流程大概就是:
```c++
//  先继承FRunnable, 实现要多线程执行的逻辑.
//  class MyRunnableTest : public FRunnable;

MyRunnableTest * MyRunnable = new MyRunnableTest();
FRunnableThread* MyThread = FRunnableThread::Create(MyRunnable, TEXT("MyRunnableTest"));

// do something 
FPlatformProcess::Sleep(5.0f);

MyThread->WaitForCompletion();

delete MyThread;
delete MyRunnable;
```
但是一般都不会直接用它, 而是基于它封装的另外两组多线程接口.

## QueuedWork

首先有一个线程池`FQueuedThreadPoolBase`, 在启动时会根据硬件条件创建一定数量的线程.这些线程会在一个队列`(QueuedThreads)`中排队等待执行任务.所有希望多线程执行的任务`IQueuedWork`都需要被添加到任务队列`QueuedWork`中排队等待执行.
![](./image/ThreadPool.png)
通常我们创建的Work都在一个全局线程池`GThreadPool`中执行, 它是在Engine的启动流程中创建的, 池中的线程数由当前机器的核心数和是否是Server决定.
```c++
// int32 FEngineLoop::PreInitPreStartupScreen(const TCHAR* CmdLine)
{
	GThreadPool = FQueuedThreadPool::Allocate();
	int32 NumThreadsInThreadPool = FPlatformMisc::NumberOfWorkerThreadsToSpawn();

	// we are only going to give dedicated servers one pool thread
	if (FPlatformProperties::IsServerOnly())
	{
		NumThreadsInThreadPool = 1;
	}
	verify(GThreadPool->Create(NumThreadsInThreadPool, StackSize * 1024, TPri_SlightlyBelowNormal, TEXT("ThreadPool")));
}
```
`FQueuedThreadPool::Allocate()`返回的就是`FQueuedThreadPoolBase`的实例.其真正的创建过程是在`FQueuedThreadPoolBase::Create`, 直接创建给定数量的线程, 并添加到`AllThreads`和空闲线程队列中`QueuedThreads`.这里并不是直接创建的`FRunnableThread`, 而是创建`FQueuedThread(FRunnable)`, 而且线程队列中持有的也是它, 真正的线程由`FQueuedThread`自己创建并复制其生命周期:
```c++
// class FQueuedThread : public FRunnable
virtual bool Create(class FQueuedThreadPoolBase* InPool,uint32 InStackSize = 0,EThreadPriority ThreadPriority=TPri_Normal)
{
	static int32 PoolThreadIndex = 0;
	const FString PoolThreadName = FString::Printf( TEXT( "PoolThread %d" ), PoolThreadIndex );
	PoolThreadIndex++;

	OwningThreadPool = InPool;
	DoWorkEvent = FPlatformProcess::GetSynchEventFromPool();
	Thread = FRunnableThread::Create(this, *PoolThreadName, InStackSize, ThreadPriority, FPlatformAffinity::GetPoolThreadMask());
	check(Thread);
	return true;
}
```
创建好的线程立即就会开始执行`FQueuedThread::Run()`:
```c++
uint32
FQueuedThread::Run()
{
	while (!TimeToDie.Load(EMemoryOrder::Relaxed)) // shutdown 流程
	{
		// This will force sending the stats packet from the previous frame.
		SET_DWORD_STAT(STAT_ThreadPoolDummyCounter, 0);
		// We need to wait for shorter amount of time
		bool bContinueWaiting = true;

		// Unless we're collecting stats there doesn't appear to be any reason to wake
		// up again until there's work to do (or it's time to die)

#if STATS
		if (FThreadStats::IsCollectingData())
		{
			while (bContinueWaiting)
			{
				DECLARE_SCOPE_CYCLE_COUNTER(TEXT("FQueuedThread::Run.WaitForWork"), STAT_FQueuedThread_Run_WaitForWork, STATGROUP_ThreadPoolAsyncTasks);

				// Wait for some work to do

				bContinueWaiting = !DoWorkEvent->Wait(GDoPooledThreadWaitTimeouts ? 10 : MAX_uint32);
			}
		}
#endif

		if (bContinueWaiting)
		{
			DoWorkEvent->Wait();
		}

		IQueuedWork* LocalQueuedWork = QueuedWork;
		QueuedWork = nullptr;
		FPlatformMisc::MemoryBarrier();
		check(LocalQueuedWork || TimeToDie.Load(EMemoryOrder::Relaxed)); // well you woke me up, where is the job or termination request?
		while (LocalQueuedWork)
		{
			// Tell the object to do the work
			LocalQueuedWork->DoThreadedWork();
			// Let the object cleanup before we remove our ref to it
			LocalQueuedWork = OwningThreadPool->ReturnToPoolOrGetNextJob(this);
		}
	}
	return 0;
}
```
至此整个线程队列就初始化完了, 随后就可以使用它多线程执行Work:
```c++
GThreadPool->AddQueuedWork(IQueuedWork* InWork);
```
其实现细节:
```c++
void AddQueuedWork(IQueuedWork* InQueuedWork) override
{
	check(InQueuedWork != nullptr);
	// 如果处于shutdown过程中, 则不会执行Work.
	if (TimeToDie)
	{
		InQueuedWork->Abandon();
		return;
	}
	check(SynchQueue);
	FQueuedThread* Thread = nullptr;
	{
		FScopeLock sl(SynchQueue);
		const int32 AvailableThreadCount = QueuedThreads.Num();
		if (AvailableThreadCount == 0)
		{
			// 没有空闲线程, 先放到Work队列中
			QueuedWork.Add(InQueuedWork);
			return;
		}
		const int32 ThreadIndex = AvailableThreadCount - 1;
		// 取最近用过的线程
		Thread = QueuedThreads[ThreadIndex];
		// Remove it from the list so no one else grabs it
		QueuedThreads.RemoveAt(ThreadIndex, 1, /* do not allow shrinking */ false);
	}
	// 有空闲线程, 直接DoWork.
	Thread->DoWork(InQueuedWork);
}
```
