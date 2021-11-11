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

			// Create the thread as suspended, so we can ensure ThreadId is initialized and the thread manager knows about the thread before it runs.
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