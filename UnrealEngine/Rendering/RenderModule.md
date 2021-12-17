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
						Create FMeshPassProcessor, Get Pass from ParallelmeshDrawCommandPasses 
                            // 对于此View此Pass创建所有DrawCommand.
                            FParallelMeshDrawCommandPass::DispatchPassSetup(View, FMeshPassProcessor)
		RenderPrePass()
			for each Views
				

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

```c++
void FParallelMeshDrawCommandPass::DispatchPassSetup(
	FScene* Scene,
	const FViewInfo& View,
	EMeshPass::Type PassType,
	FExclusiveDepthStencil::Type BasePassDepthStencilAccess,
	FMeshPassProcessor* MeshPassProcessor,
	const TArray<FMeshBatchAndRelevance, SceneRenderingAllocator>& DynamicMeshElements,
	const TArray<FMeshPassMask, SceneRenderingAllocator>* DynamicMeshElementsPassRelevance,
	int32 NumDynamicMeshElements,
	TArray<const FStaticMeshBatch*, SceneRenderingAllocator>& InOutDynamicMeshCommandBuildRequests,
	int32 NumDynamicMeshCommandBuildRequestElements,
	FMeshCommandOneFrameArray& InOutMeshDrawCommands,
	FMeshPassProcessor* MobileBasePassCSMMeshPassProcessor,
	FMeshCommandOneFrameArray* InOutMobileBasePassCSMMeshDrawCommands
)
{
	// Setup TaskContext

	TaskEventRef = TGraphTask<FMeshDrawCommandPassSetupTask>::CreateTask(nullptr, ENamedThreads::GetRenderThread()).ConstructAndDispatchWhenReady(TaskContext);
	// 
		GenerateDynamicMeshDrawCommands(
		*Context.View,
		Context.ShadingPath,
		Context.PassType,
		Context.MeshPassProcessor,
		*Context.DynamicMeshElements,
		Context.DynamicMeshElementsPassRelevance,
		Context.NumDynamicMeshElements,
		Context.DynamicMeshCommandBuildRequests,
		Context.NumDynamicMeshCommandBuildRequestElements,
		Context.MeshDrawCommands,
		Context.MeshDrawCommandStorage,
		Context.MinimalPipelineStatePassSet,
		Context.NeedsShaderInitialisation
	);
		ApplyViewOverridesToMeshDrawCommands(
		Context.ShadingPath,
		Context.PassType,
		Context.bReverseCulling,
		Context.bRenderSceneTwoSided,
		Context.BasePassDepthStencilAccess,
		Context.DefaultBasePassDepthStencilAccess,
		Context.MeshDrawCommands,
		Context.MeshDrawCommandStorage,
		Context.MinimalPipelineStatePassSet,
		Context.NeedsShaderInitialisation,
		Context.TempVisibleMeshDrawCommands
	);
		UpdateTranslucentMeshSortKeys()	
			for each VisibleCommand in VisibleMeshCommands
				udpate VisibleCommand.SortKey.PackedData = SortKey.PackedData;

		BuildMeshDrawCommandPrimitiveIdBuffer()
	TaskEventRef = TGraphTask<FMeshDrawCommandInitResourcesTask>::CreateTask(&DependentGraphEvents, ENamedThreads::GetRenderThread()).ConstructAndDispatchWhenReady(TaskContext);
		for each Initializer in Context.MinimalPipelineStatePassSet
			Initializer.BoundShaderState.LazilyInitShaders();
}

GenerateDynamicMeshDrawCommands(
	const FViewInfo& View,
	EShadingPath ShadingPath,
	EMeshPass::Type PassType,
	FMeshPassProcessor* PassMeshProcessor,
	const TArray<FMeshBatchAndRelevance, SceneRenderingAllocator>& DynamicMeshElements,
	const TArray<FMeshPassMask, SceneRenderingAllocator>* DynamicMeshElementsPassRelevance,
	int32 MaxNumDynamicMeshElements,
	const TArray<const FStaticMeshBatch*, SceneRenderingAllocator>& DynamicMeshCommandBuildRequests,
	int32 MaxNumBuildRequestElements,
	FMeshCommandOneFrameArray& VisibleCommands, // 最终的DrawCommand
	FDynamicMeshDrawCommandStorage& MeshDrawCommandStorage,
	FGraphicsMinimalPipelineStateSet& MinimalPipelineStatePassSet,
	bool& NeedsShaderInitialisation
)
{
	FDynamicPassMeshDrawListContext DynamicPassMeshDrawListContext(
		MeshDrawCommandStorage,
		VisibleCommands,
		MinimalPipelineStatePassSet,
		NeedsShaderInitialisation
	);
	PassMeshProcessor->SetDrawListContext(&DynamicPassMeshDrawListContext);
	for each MeshAndRelevance in DynamicMeshElements
		PassMeshProcessor->AddMeshBatch(*MeshAndRelevance.Mesh, BatchElementMask, MeshAndRelevance.PrimitiveSceneProxy);
	for StaticMeshBatch in DynamicMeshCommandBuildRequests
		PassMeshProcessor->AddMeshBatch(*StaticMeshBatch, DefaultBatchElementMask, StaticMeshBatch->PrimitiveSceneInfo->Proxy, StaticMeshBatch->Id);
}
// For example : 
void FBasePassMeshProcessor::AddMeshBatch(const FMeshBatch& RESTRICT MeshBatch, uint64 BatchElementMask, const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, int32 StaticMeshId)
	Process< FUniformLightMapPolicy >(
								MeshBatch,
								BatchElementMask,
								StaticMeshId,
								PrimitiveSceneProxy,
								MaterialRenderProxy,
								Material,
								BlendMode,
								ShadingModels,
								FUniformLightMapPolicy(LMP_DISTANCE_FIELD_SHADOWS_AND_HQ_LIGHTMAP),
								MeshBatch.LCI,
								MeshFillMode,
								MeshCullMode);
		GetBasePassShaders<LightMapPolicyType>(FMaterial); // 
		SetDepthStencilStateForBasePass();
		BuildMeshDrawCommands(); 	

```