```c++
#define ENQUEUE_RENDER_COMMAND(Type) \
struct Type##Name \
{  \
	static const char* CStr() { return #Type; } \
	static const TCHAR* TStr() { return TEXT(#Type); } \
}; \
EnqueueUniqueRenderCommand<Type##Name>

// 一个函数调用
ENQUEUE_RENDER_COMMAND(FDrawSceneCommand) --->>> 展开为:
// 性能统计用
struct FDrawSceneCommandName 
{  
	static const char* CStr() { return "FDrawSceneCommand"; } 
	static const TCHAR* TStr() { return TEXT("FDrawSceneCommand"); } 
}; 
EnqueueUniqueRenderCommand<FDrawSceneCommandName>(
			[SceneRenderer, DrawSceneEnqueue](FRHICommandListImmediate& RHICmdList)
			{
				const float StartDelayMillisec = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles() - DrawSceneEnqueue);
				CSV_CUSTOM_STAT_GLOBAL(DrawSceneCommand_StartDelay, StartDelayMillisec, ECsvCustomStatOp::Set);

				RenderViewFamily_RenderThread(RHICmdList, SceneRenderer);
				FlushPendingDeleteRHIResources_RenderThread();
			});
```
其中`EnqueueUniqueRenderCommand`:
```c++
template<typename TSTR, typename LAMBDA>
FORCEINLINE_DEBUGGABLE void EnqueueUniqueRenderCommand(LAMBDA&& Lambda)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_EnqueueUniqueRenderCommand);
    // DoTask实现, 期望在RenderThread执行, 不支持被依赖
	typedef TEnqueueUniqueRenderCommandType<TSTR, LAMBDA> EURCType;
    // 在渲染线程Enqueue则直接执行.
	if (IsInRenderingThread())
	{
		FRHICommandListImmediate& RHICmdList = GetImmediateCommandList_ForRenderCommand();
		Lambda(RHICmdList);
	}
	else
	{
        // 如果是多线程渲染则应该在Render线程执行
        // 则创建一个Task.
		if (ShouldExecuteOnRenderThread()) // GIsThreadedRendering || !IsInGameThread()
		{
			CheckNotBlockedOnRenderThread();
            // 创建Task, 放到指定线程的Task队列中等待执行
			TGraphTask<EURCType>::CreateTask().ConstructAndDispatchWhenReady(Forward<LAMBDA>(Lambda));
		}
		else // 立即执行, 跳过Task创建.
		{
			EURCType TempCommand(Forward<LAMBDA>(Lambda));
			FScopeCycleCounter EURCMacro_Scope(TempCommand.GetStatId());
			TempCommand.DoTask(ENamedThreads::GameThread, FGraphEventRef());
		}
	}
}
```
这里处理了各种执行情况, 单线程, 多线程, 根据当前线程跳过TaskGraph的流程等等.但不论哪一种情况, 执行方式都是一样的:
```c++
FRHICommandListImmediate& RHICmdList = GetImmediateCommandList_ForRenderCommand();
Lambda(RHICmdList);
```