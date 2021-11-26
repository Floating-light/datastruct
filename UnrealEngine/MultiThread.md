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
FRunnableThreadHoloLens
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
`QueuedWork`的基本想法是, 将需要多线程执行的任务放到一个任务队列中, 在线程池中的线程取出这些任务并执行.通常的用法, 首先需要实现需要多线程执行的Task:

```c++
class FExampleAsyncTask : public FNonAbandonableTask
{
	friend class FAsyncTask<FExampleAsyncTask>;
	int32 ExampleData;
	FExampleAsyncTask(int32 InData)
		: ExampleData(InData)
	{

	}
	void DoWork()
	{
		double Begin = FPlatformTime::Seconds();

		UE_LOG(LogTemp, Display, TEXT("Begin DoWork Thread  : %s,Begin Seconds : %f "), *FThreadManager::Get().GetThreadName(FPlatformTLS::GetCurrentThreadId()),  Begin);
		int MyRes = factorial(ExampleData);
		UE_LOG(LogTemp, Display, TEXT("End DoWork Thread  : %s, Time cost : %f, The factorial of %d is %d  "), *FThreadManager::Get().GetThreadName(FPlatformTLS::GetCurrentThreadId()),  FPlatformTime::Seconds() - Begin, ExampleData, MyRes);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyExampleAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
private:
	int factorial(int N)
	{
		int res = 1;
		for (int i = N; i > 1; i--)
		{
			res *= i;
			FPlatformProcess::Sleep(1);
		}
		return res;
	}
};
```
使用时:
```c++
FAsyncTask<FExampleAsyncTask>* MyTask = new FAsyncTask<FExampleAsyncTask>(10);
// 开始执行
MyTask->StartBackgroundTask();

// 可以在Tick中轮询
if (MyTask->IsDone())
{

}

MyTask->EnsureCompletion(); // 等待直到完成
{
	UE_LOG(LogPlayerController, Display, TEXT("Completion callback : %s "), * FThreadManager::Get().GetThreadName(FPlatformTLS::GetCurrentThreadId()));
}
delete MyTask;
```
`FAsyncTask`是Engine实现好的一种`IQueuedWork`, 提供了Work完成相关的同步操作, 当确保Work执行完之后要手动清除这个任务(或者重复利用). 

还有一种`FAutoDeleteAsyncTask`, 会在执行完之后自动`delete this`(从Pool thread).
```c++
// FAutoDeleteAsyncTask::DoWork
void DoWork()
{
	LLM_SCOPE(InheritedLLMTag);
	FScopeCycleCounter Scope(Task.GetStatId(), true);

	Task.DoWork();
	delete this;
}
```
### 实现
首先有一个线程池`FQueuedThreadPoolBase`, 在启动时会根据硬件条件创建一定数量的线程. 这些线程会在一个队列`(QueuedThreads)`中排队等待执行任务.所有希望多线程执行的任务`IQueuedWork`都需要被添加到任务队列`QueuedWork`中排队等待执行.
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
`FQueuedThreadPool::Allocate()`返回的就是`FQueuedThreadPoolBase`的实例.其真正的创建过程是在`FQueuedThreadPoolBase::Create`, 直接创建给定数量的线程, 并添加到`AllThreads`和空闲线程队列中`QueuedThreads`.这里并不是直接创建的`FRunnableThread`, 而是创建`FQueuedThread(FRunnable)`, 而且线程队列中持有的也是它, 真正的线程由`FQueuedThread`自己创建并负责其生命周期:
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
		// 仅在收集统计数据时才有可能在等待Work的中途唤醒.
#if STATS
		if (FThreadStats::IsCollectingData())
		{
			while (bContinueWaiting)
			{
				DECLARE_SCOPE_CYCLE_COUNTER(TEXT("FQueuedThread::Run.WaitForWork"), STAT_FQueuedThread_Run_WaitForWork, STATGROUP_ThreadPoolAsyncTasks);

				// GDoPooledThreadWaitTimeouts 是一个可配置的控制台变量, 默认false, 
				bContinueWaiting = !DoWorkEvent->Wait(GDoPooledThreadWaitTimeouts ? 10 : MAX_uint32);
			}
		}
#endif
		if (bContinueWaiting)
		{
			// 在主线程中设置好`QueuedWork`之后触发
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
			// 如果Work队列中有Work, 则给一个继续执行
			// 没有 则加入到线程池空闲线程队列中, 并返回nullptr, 进入等待状态(等待触发DoWorkEvent)
			LocalQueuedWork = OwningThreadPool->ReturnToPoolOrGetNextJob(this);
		}
	}
	return 0;
}
```
创建好的`FQueuedThread`只存在两种状态, 要么被放在QueuedThread队列中, 处于空闲状态. 要么处于忙碌状态, 不停地从任务队列中取出任务来执行.

`ReturnToPoolOrGetNextJob(this)`会尝试从任务队列中拿出一个Work继续执行, 如果没有, 则把传入的Thread加入到空闲线程队列中, 并进入休眠状态, 直到下一次调用`DoWork()`通过事件唤醒.

```c++
IQueuedWork* ReturnToPoolOrGetNextJob(FQueuedThread* InQueuedThread)
{
	check(InQueuedThread != nullptr);
	IQueuedWork* Work = nullptr;
	// Check to see if there is any work to be done
	FScopeLock sl(SynchQueue);
	if (TimeToDie)
	{
		check(!QueuedWork.Num());  // we better not have anything if we are dying
	}
	if (QueuedWork.Num() > 0) // 尝试获取一个可以执行的Work
	{
		Work = QueuedWork[0];
		QueuedWork.RemoveAt(0, 1, /* do not allow shrinking */ false);
	}
	if (!Work) // 没有可执行任务时, 加入到空闲线程队列中
	{
		QueuedThreads.Add(InQueuedThread);
	}
	return Work;
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
由于空闲线程队列中的线程都处于休眠状态`(DoWorkEvent->Wait())`, `DoWorkEvent->Trigger()`会唤醒当前thread, Work线程主循环和DoWork()对`QueuedWork`的读写都没有加锁, 这是在逻辑上保证了它们不会冲突. 为确保在多处理器情况下的写入同步, 每次对`QueuedWork`的更改后都需要调用`FPlatformMisc::MemoryBarrier()`, 以确保当前写入对其它处理器可见.
```c++
void DoWork(IQueuedWork* InQueuedWork)
{
	DECLARE_SCOPE_CYCLE_COUNTER( TEXT( "FQueuedThread::DoWork" ), STAT_FQueuedThread_DoWork, STATGROUP_ThreadPoolAsyncTasks );

	check(QueuedWork == nullptr && "Can't do more than one task at a time");
	// Tell the thread the work to be done
	QueuedWork = InQueuedWork;
	FPlatformMisc::MemoryBarrier();
	// Tell the thread to wake up and do its job
	DoWorkEvent->Trigger();
}
```

### IQueuedWork
在实现具体的`Work`时, 常常还需要考虑任务执行完的同步的问题, 所以一般不直接使用`IQueuedWork`, 而是其派生的`FAsyncTask<TTask>`, 可以方便地查询任务是否完成, 或等待任务完成.`TAsyncQueuedWork<ResultType>`可以多线程执行一个函数并获取它的返回值.

## TaskGraph
相比于QueuedWork, TaskGraph中的任务之间还可以指定依赖关系(不能循环依赖), 指定前序任务和后序任务.

`FBaseGraphTask`主要完成Task多线程执行的逻辑, 即将自己加入到TaskGraphInterface中指定线程的任务队列中,与FTaskGraphInterface的耦合.

```c++
class FBaseGraphTask
{
protected:
	void SetThreadToExecuteOn(ENamedThreads::Type InThreadToExecuteOn)
	{
		ThreadToExecuteOn = InThreadToExecuteOn;
		checkThreadGraph(LifeStage.Increment() == int32(LS_ThreadSet));
	}

	// 完成前置条件的配置, 在这之后, 只要 NumberOfPrerequistitesOutstanding为0,就可以执行.
	void PrerequisitesComplete(ENamedThreads::Type CurrentThread, int32 NumAlreadyFinishedPrequistes, bool bUnlock = true)
	{
		checkThreadGraph(LifeStage.Increment() == int32(LS_PrequisitesSetup));
		int32 NumToSub = NumAlreadyFinishedPrequistes + (bUnlock ? 1 : 0); // the +1 is for the "lock" we set up in the constructor
		if (NumberOfPrerequistitesOutstanding.Subtract(NumToSub) == NumToSub) 
		{
			QueueTask(CurrentThread);
		}
	}

	// 每当有一个前置任务完成时, this 作为其后置任务, 会在这个完成的GraphEvent中调用所有后置任务的这个函数.
	// 直到计数器减为0, 表明所有前置任务执行完毕, 这个任务可以开始执行.
	void ConditionalQueueTask(ENamedThreads::Type CurrentThread)
	{
		if (NumberOfPrerequistitesOutstanding.Decrement()==0)
		{
			QueueTask(CurrentThread);
		}
	}
private:
	// Subclass API

	// 子类实现执行Task逻辑的地方
	virtual void ExecuteTask(TArray<FBaseGraphTask*>& NewTasks, ENamedThreads::Type CurrentThread)=0;

	// 在所要求的线程Worker中调用, 执行Task
	FORCEINLINE void Execute(TArray<FBaseGraphTask*>& NewTasks, ENamedThreads::Type CurrentThread)
	{
		LLM_SCOPE(InheritedLLMTag);
		checkThreadGraph(LifeStage.Increment() == int32(LS_Executing));
		ExecuteTask(NewTasks, CurrentThread);
	}

	// Internal Use

	// 把自己放到对应线程的任务队列中.
	void QueueTask(ENamedThreads::Type CurrentThreadIfKnown)
	{
		checkThreadGraph(LifeStage.Increment() == int32(LS_Queued));
		FTaskGraphInterface::Get().QueueTask(this, ThreadToExecuteOn, CurrentThreadIfKnown);
	}

	/**	Thread to execute on, can be ENamedThreads::AnyThread to execute on any unnamed thread **/
	ENamedThreads::Type			ThreadToExecuteOn;
	/**	Number of prerequisites outstanding. When this drops to zero, the thread is queued for execution.  **/
	FThreadSafeCounter			NumberOfPrerequistitesOutstanding; 
};
```

`FGraphEvent`表示一个Task完成的事件.所以`FGraphEvent`总是和一个Task相关, 它也是在一个Task初始化的时候创建的.`FGraphEvent`实现了Task之间的依赖关系, 在一个Task执行完成之后, 与其相关的Event就算完成了, 马上Event就会处理所有依赖于自己的后续Task(FBaseGraphTask->ConditionalQueueTask).

```c++
class FGraphEvent 
{
public:
	// 添加一个任务作为后续任务, 如果此事件已经Fired, 将返回false, 表明此事件无法作为前置事件.
	// 通常一个被依赖的事件会作为参数, 其后序Task构造时传入, 调用PrerequisiteEvent->AddSubsequent(this)
	bool AddSubsequent(class FBaseGraphTask* Task)
	{
		return SubsequentList.PushIfNotClosed(Task);
	}

	// 给定一个事件作为当前事件完成的条件之一.
	// 仅在执行关联的Task期间有效.也就是说一个Task在执行时可以添加自己的子Task, 且仅当子Task完成时自己才算完成.
	void DontCompleteUntil(FGraphEventRef EventToWaitFor)
	{
		checkThreadGraph(!IsComplete()); 
		new (EventsToWaitFor) FGraphEventRef(EventToWaitFor);
	}

	// 在关联的Task执行完成之后调用, 会调用所有后续Task(SubsequentList)的`ConditionalQueueTask`, 将这些Task的前置任务计数减一, 为0时就queue
	CORE_API void DispatchSubsequents(ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread);

	// 和前面一样, 只是将NewTasks也作为后续Task.
	CORE_API void DispatchSubsequents(TArray<FBaseGraphTask*>& NewTasks, ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread);

    // 判断是否完成.
	bool IsComplete() const
	{
		return SubsequentList.IsClosed();
	}

	void Wait(ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
	{
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(this, CurrentThreadIfKnown);
	}
private:

	// 线程安全的无锁Subsequents链表
	TClosableLockFreePointerListUnorderedSingleConsumer<FBaseGraphTask, 0>	SubsequentList;
	// 非线程安全, 仅在执行Task的Context下添加 
	FGraphEventArray														EventsToWaitFor;

	FThreadSafeCounter														ReferenceCount;
	ENamedThreads::Type														ThreadToDoGatherOn;
};

```
`EventsToWaitFor`是在执行Task的业务逻辑(FMyTask::DoTask)中添加的子Task, 

```c++
FGraphEventRef Task1Evet = TGraphTask<FMyTask>::CreateTask(nullptr, ENamedThreads::AnyThread).ConstructAndDispatchWhenReady(2,100000);
```
`TGraphTask<TTask>::CreateTask`返回的是一个Helper对象, 用于进一步构造这个Task.

```c++
static FConstructor CreateTask(const FGraphEventArray* Prerequisites = NULL, ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)
{
	int32 NumPrereq = Prerequisites ? Prerequisites->Num() : 0;
	if (sizeof(TGraphTask) <= FBaseGraphTask::SMALL_TASK_SIZE)
	{
		void *Mem = FBaseGraphTask::GetSmallTaskAllocator().Allocate();
		return FConstructor(new (Mem) TGraphTask(TTask::GetSubsequentsMode() == ESubsequentsMode::FireAndForget ? NULL : FGraphEvent::CreateGraphEvent(), NumPrereq), Prerequisites, CurrentThreadIfKnown);
	}
	return FConstructor(new TGraphTask(TTask::GetSubsequentsMode() == ESubsequentsMode::FireAndForget ? NULL : FGraphEvent::CreateGraphEvent(), NumPrereq), Prerequisites, CurrentThreadIfKnown);
}
```
1. 如果当前Task的大小(< 256 bytes), 选择用自定义的Allocator, 否则直接new.
2. 如果TTask::GetSubsequentsMode() 不是FireAndForget就创建FGraphEvent.
3. 将new出来的TGraphTask传给FConstructor并返回.

对返回的`FConstructor`, 可以调用`ConstructAndDispatchWhenReady`或`ConstructAndHold`, 它们都需要传入模板参数TTask的构造参数, 并都会用`Placement new`在前一步构造的FGraphTask的一块内存中构造TTask:
```c++
template<typename...T>
FGraphEventRef ConstructAndDispatchWhenReady(T&&... Args)
{
	new ((void *)&Owner->TaskStorage) TTask(Forward<T>(Args)...);
	return Owner->Setup(Prerequisites, CurrentThreadIfKnown);
}

template<typename...T>
TGraphTask* ConstructAndHold(T&&... Args)
{
	new ((void *)&Owner->TaskStorage) TTask(Forward<T>(Args)...);
	return Owner->Hold(Prerequisites, CurrentThreadIfKnown);
}
```
其中`TaskStorage`就是一块`uint8[sizeof(TTask)]`的内存, 在`new TGraphTask`时就一起创建好了.
```c++
TAlignedBytes<sizeof(TTask),alignof(TTask)> TaskStorage;

// -------------------
template<int32 Size>
struct TAlignedBytes<Size,1>
{
	uint8 Pad[Size];
};
```
这两个函数不同的地方在于是调用TGraphTask的Setup()或Hold().而它们唯一的区别在于, Hold()会手动给前置依赖任务的计数器加上1, 不管依赖任务如何, 都先不执行, 直到显式调用`TGraphTask<FMyTask>->Unlock()`, 将计数器减一, 才开始执行(如果满足其它前置条件).`Setup()`在满足前置条件的情况下会直接执行.


```c++
// Setup() : bUnlock = true, Hold() : bUnlock = false
void SetupPrereqs(const FGraphEventArray* Prerequisites, ENamedThreads::Type CurrentThreadIfKnown, bool bUnlock)
{
	checkThreadGraph(!TaskConstructed);
	TaskConstructed = true;
	TTask& Task = *(TTask*)&TaskStorage;
	SetThreadToExecuteOn(Task.GetDesiredThread());
	int32 AlreadyCompletedPrerequisites = 0;
	if (Prerequisites)
	{
		for (int32 Index = 0; Index < Prerequisites->Num(); Index++)
		{
			FGraphEvent* Prerequisite = (*Prerequisites)[Index];
			if (Prerequisite == nullptr || !Prerequisite->AddSubsequent(this))
			{
				AlreadyCompletedPrerequisites++;
			}
		}
	}
	PrerequisitesComplete(CurrentThreadIfKnown, AlreadyCompletedPrerequisites, bUnlock);
}
```

在满足依赖条件之后, Task会被加入到TaskGraph中执行:
```c++
void QueueTask(ENamedThreads::Type CurrentThreadIfKnown)
{
	checkThreadGraph(LifeStage.Increment() == int32(LS_Queued));
	FTaskGraphInterface::Get().QueueTask(this, ThreadToExecuteOn, CurrentThreadIfKnown);
}
```

### FTaskGraphInterface

`FTaskGraphInterface`定义了向外部提供的一组功能API, 真正的实现在`FTaskGraphImplementation`, 其创建在`Engine`初始化阶段

```c++
//  LaunchEngineLoop.cpp Line: 2018
FTaskGraphInterface::Startup(FPlatformMisc::NumberOfCores());
FTaskGraphInterface::Get().AttachToThread(ENamedThreads::GameThread);
```
`Startup()`主要作用是创建指定数量的`WorkerThreads`,一个Worker由一个Runnable对象和一个前面提到的FRunnableThread组成.
```c++
struct FWorkerThread
{
	FTaskThreadBase*	TaskGraphWorker;
	FRunnableThread*	RunnableThread;
	bool				bAttached;
};
```
TaskGraph中的工作线程主要分两种, 一种是NamedThread(external thread)和UnamedThread(也称作AnyThread, internal thread),NamedThread是有着明确的意义的Thread, 其中通常只执行一类任务, 目前有下面5种:
```c++
StatsThread, 
RHIThread,
AudioThread,
GameThread,
ActualRenderingThread = GameThread + 1,
``` 
`TaskGraph`中一定会创建这几个基本NamedThread, 并至少创建一个UnamedThread, 在此基础上, 核心数越多UnamedThread的数量越多.
首先会创建所有WorkerThreads的Runnable:
```c++
if (bAnyTaskThread)
{
	WorkerThreads[ThreadIndex].TaskGraphWorker = new FTaskThreadAnyThread(ThreadIndexToPriorityIndex(ThreadIndex));
}
else
{
	WorkerThreads[ThreadIndex].TaskGraphWorker = new FNamedTaskThread;
}
```
显然NamedThread和AnyThread有着不同的FRunnable的实现.随后, 对于UnamedThread会马上创建对应的`FRunnableThread`, 创建好后, 这些Worker马上就进入等待执行任务的状态:
```c++
WorkerThreads[ThreadIndex].RunnableThread = FRunnableThread::Create(&Thread(ThreadIndex), *Name, StackSize, ThreadPri, Affinity); 

WorkerThreads[ThreadIndex].bAttached = true;
```
在`FTaskGraphInterface::Startup()`之后, 基本上就完成了TaskGraph的初始化.


FTaskGraphInterface中除了和Startup, shutdown, singleton相关的API之外, 还有一组操作NamedThread的执行逻辑的API. 对于NamedThread的Worker,其真正的Thread对象都是在外部相应的功能模块创建的, 甚至GameThread就是主线程本身, 所以NamedThreadWorker是没有Thread对象的,FTaskGraphInterface更像是仅仅管理了它们对应的Task队列, 所有需要NamedThread执行的任务, 可以通过`QueueTask`添加到对应Worker的Task队列中, 而这些真正的外部的线程, 在自己恰当的时机通过`FTaskGraphInterface`的API执行自己队列中的Task.

```c++
class FTaskGraphInterface
{
	friend class FBaseGraphTask;
	// 从FBaseGraphTask, 在满足前置任务条件后调用.
	virtual void QueueTask(class FBaseGraphTask* Task, ENamedThreads::Type ThreadToExecuteOn, ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread) = 0;

public:
	// Startup, shutdown and singleton API

	// 创建指定数量的Worker, 保证数量比所有NamedThread的数量多1.
	static CORE_API void Startup(int32 NumThreads);
	// Destruct 所有Worker
	static CORE_API void Shutdown();
	static CORE_API FTaskGraphInterface& Get();
	
	// 以下仅用于外部Thread(真NamedThread)
	// 将当前线程和NamedThread关联, 通常只初始化TLS信息.
	virtual void AttachToThread(ENamedThreads::Type CurrentThread)=0;

	// 处理一个NamedThread中的所有任务, 然后返回. 
	virtual uint64 ProcessThreadUntilIdle(ENamedThreads::Type CurrentThread)=0;

	// 进入一个NamedThread的处理Work的循环, 直到显式要求返回
	virtual void ProcessThreadUntilRequestReturn(ENamedThreads::Type CurrentThread)=0;

	// 要求当处于空闲状态时, 从处于执行任务的循环的状态返回.
	virtual void RequestReturn(ENamedThreads::Type CurrentThread)=0;

	// 等待`Tasks`全部执行完再返回
	virtual void WaitUntilTasksComplete(const FGraphEventArray& Tasks, ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread)=0;

	// 当Tasks中的任务完成时,触发事件`InEvent` 
	virtual void TriggerEventWhenTasksComplete(FEvent* InEvent, const FGraphEventArray& Tasks, ENamedThreads::Type CurrentThreadIfKnown = ENamedThreads::AnyThread, ENamedThreads::Type TriggerThread = ENamedThreads::AnyHiPriThreadHiPriTask)=0;
};
```
比如GameThread的Task, 是在Game主循环的每一帧末尾同步的时候:
```c++
bool bEmptyGameThreadTasks = !FTaskGraphInterface::Get().IsThreadProcessingTask(ENamedThreads::GameThread);

if (bEmptyGameThreadTasks)
{
	// need to process gamethread tasks at least once a frame no matter what
	FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
}
```
这里会找到之前创建的GameThread 的Worker, 把需要GameThread执行的队列中的所有Task执行完毕才会返回.


例如AudioThread, 其在外部(Audio模块)创建了它的Thread对象和对应的Runnable, 在AudioThreadRunnable的Run()函数中调用了
```c++
void AudioThreadMain( FEvent* TaskGraphBoundSyncEvent )
{
	FTaskGraphInterface::Get().AttachToThread(ENamedThreads::AudioThread);
	FPlatformMisc::MemoryBarrier();

	// Inform main thread that the audio thread has been attached to the taskgraph and is ready to receive tasks
	if( TaskGraphBoundSyncEvent != nullptr )
	{
		TaskGraphBoundSyncEvent->Trigger();
	}

	FTaskGraphInterface::Get().ProcessThreadUntilRequestReturn(ENamedThreads::AudioThread);
	FPlatformMisc::MemoryBarrier();
}
```

而非Named Thread在`FTaskGraphInterface`中再无其它API, 我们仅需通过创建在AnyThread中执行的Task, 这些Task会被统一放到`FTaskGraphImplementation`的同一个队列`IncomingAnyThreadTasks`中, UnnamedThread会在自己的循环中执行它们, 这里UnnamedThread的行为和前面的`QueuedThreadPool`就很像了.


* https://stackoverflow.com/questions/4537753/when-should-i-use-mm-sfence-mm-lfence-and-mm-mfence

* https://zhuanlan.zhihu.com/p/38881269

* https://michaeljcole.github.io/wiki.unrealengine.com/Multi-Threading:_Task_Graph_System/

* https://zhuanlan.zhihu.com/p/57928032