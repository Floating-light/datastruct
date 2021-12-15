```c++
void FRendererModule::BeginRenderingViewFamily(FCanvas* Canvas, FSceneViewFamily* ViewFamily) // Game Thread , Frame
    FSceneRenderer* SceneRenderer = FSceneRenderer::CreateSceneRenderer(ViewFamily, Canvas->GetHitProxyConsumer());
        	if (ShadingPath == EShadingPath::Deferred)
	        {
	        	SceneRenderer = new FDeferredShadingSceneRenderer(InViewFamily, HitProxyConsumer);
                // 初始化 Scene, ViewFamily,  Views
	        }
	        else 
	        {
	        	check(ShadingPath == EShadingPath::Mobile);
	        	SceneRenderer = new FMobileSceneRenderer(InViewFamily, HitProxyConsumer);
	        }
    ENQUEUE_RENDER_COMMAND(FDrawSceneCommand)(
	[SceneRenderer, DrawSceneEnqueue](FRHICommandListImmediate& RHICmdList)
	{
		const float StartDelayMillisec = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles() - DrawSceneEnqueue);
		CSV_CUSTOM_STAT_GLOBAL(DrawSceneCommand_StartDelay, StartDelayMillisec, ECsvCustomStatOp::Set);

		RenderViewFamily_RenderThread(RHICmdList, SceneRenderer);
		FlushPendingDeleteRHIResources_RenderThread();
	});
```

```c++
static void RenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneRenderer* SceneRenderer)
    SceneRenderer->Render(RHICmdList);
        InitViews();
            ComputeViewVisibility();
                GatherDynamicMeshElements()
                // ... .. 
                for each view
                    SetupMeshPass(View, BasePassDepthStencilAccess, ViewCommands);  
                        for each EMeshPass
                            // 对于此View此Pass创建所有DrawCommand.
                            FParallelMeshDrawCommandPass::DispatchPassSetup(View, FMeshPassProcessor)

```

```c++
void FSceneRenderer::GatherDynamicMeshElements(
	TArray<FViewInfo>& InViews, 
	const FScene* InScene, 
	const FSceneViewFamily& InViewFamily, 
	FGlobalDynamicIndexBuffer& DynamicIndexBuffer, // FSceneRenderer 的静态变量传入
	FGlobalDynamicVertexBuffer& DynamicVertexBuffer,
	FGlobalDynamicReadBuffer& DynamicReadBuffer,
	const FPrimitiveViewMasks& HasDynamicMeshElementsMasks, // ComputeViewVisibility() 中计算
	const FPrimitiveViewMasks& HasDynamicEditorMeshElementsMasks,
	FMeshElementCollector& Collector) // FSceneRenderer 成员变量
{
    for PrimitiveSceneInfo : InScene->Primitives
        Collector.SetPrimitive(PrimitiveSceneInfo->Proxy, PrimitiveSceneInfo->DefaultDynamicHitProxyId);
        // 在具体的FPrimitiveSceneProxy中实现, 向Collector中添加FMeshBatch.
        PrimitiveSceneInfo->Proxy->GetDynamicMeshElements(InViewFamily.Views, InViewFamily, ViewMaskFinal, Collector);
        for each Views
            for each (new MeshBatch in View.DynamicMeshElements)
                // 根据ViewRelevance的配置添加到对应Vew.*.MeshBatches数组中, 并标记(PassRelevance)都在哪些Pass中渲染
                // Collector->MeshBatches和对应View的View.DynamicMeshElements相同.
                ComputeDynamicMeshRelevance(ShadingPath, bAddLightmapDensityCommands, ViewRelevance, MeshBatch, View, PassRelevance, PrimitiveSceneInfo, Bounds);
    MeshCollector.ProcessTasks();
}
```
