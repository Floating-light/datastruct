reference: 
* https://medium.com/@lordned/unreal-engine-4-rendering-part-4-the-deferred-shading-pipeline-389fc0175789
* https://www.zhihu.com/column/c_187975189
* https://docs.unrealengine.com/4.26/zh-CN/ProgrammingAndScripting/Rendering/Overview/
* 4.22 Change : https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/Rendering/MeshDrawingPipeline/4_22_ConversionGuide/

## 3. The Deferred Shading Pipeline
```
https://medium.com/@lordned/unreal-engine-4-rendering-part-4-the-deferred-shading-pipeline-389fc0175789
```
### The Deferred Shading Base Pass

Vertex Factory 如何控制到Common base pass vertex shader的输入 ?

Tessellations 是如何处理的(Hull, Domain stages) ? 

Material Graph 最终是如何到HLSL Code中的?

Deferred pass 是如何Work的?

### A Second Look at Vertex Factories
以LocalVertexFactory.ush和 BasePassVertexCommon.ush为例. 对比GpuSkinVertexFactory.ush.

### Changing Input Data
CPU端, 用FVertexFactory处理不同类型的Mesh有着不同的顶点数据传递给GPU.

GPU端, 由于所有VertexFactories都用同样的VertexShader(至少BasePass是), 所以用一个通用命名的结构体FVertexFactoryInput来将这些数据传输到GPU. 每一种VertexFactory的shader factory中都定义自己的FVertexFactoryInput的具体实现, BasePassVertexCommon.ush中Include /Engine/Generated/VertexFactory.ush.这个文件在shader编译的时候会include到对应的*VertexFactory.ush, 其中就定义了对应的FVertexFactoryInput结构体.

此前提到, 在实现一种Mesh的VertexFactory时, 还需要用一个宏将这个VertexFactory和一个Shader代码的文件关联起来, 这个文件
```c++
IMPLEMENT_VERTEX_FACTORY_TYPE_EX(FLocalVertexFactory,"/Engine/Private/LocalVertexFactory.ush",true,true,true,true,true,true,true);
```
就是上面提到的VertexFactory的HLSL版本, 其中定义了GPU端的顶点数据表示FVertexFactoryInput.

这样, BasePassVertexShader就匹配上了顶点数据.

不同的VertexFactories在VS和PS之间需要不同的数据插值 ?

和FVertesFactoryInput的思路一样, BasePassVertexShader.usf中也会调用一些Generic function--GetVertexFactoryIntermediates, VertexFactoryGetWorldPostion, GetMaterialVertexParameters, 这些和FVertexFactoryInput一样都实现在对应的*VertexFactory.ush中.

### Changing Output Data
从VertexShader到PixelShader的输出数据, 同样的套路, 在BasePassVertexShader.usf, 用另一个Generically named struct FBasePassVSOutput, 它的实现同样也是看VertexFactory. 这里还有另一个障碍, 如果开启了Tessellation, 在Vertex Shader和Pixel shader之间还有两个阶段(Hull and Domain Stages), 这两个阶段需要不同数据(和仅仅是VS 到PS相比).

在BasePassVertexCommon.ush中, 用宏定义改变FBasePassVSOutput的定义, 根据是否开启了Tessellation, 选择FBasePassVSToDS或FBasePassVSToPS.

在最终的FBasePassVSOutput中, 有两个结构体成员FVertexFactoryInterpolantsVSToPS 和 FBasePassInterpolantsVSToPS(或DS版本), 其中, FVertexFactoryInterpolantsVSToPS是在具体的*VertexFactory.ush中定义的, 另外一个就是BasePassVertexShader通用的输出.

在BasePassVertexShader中, 用不同的inlcude, 重定义结构体和一些函数抽象出通用代码, 而不依赖于具体的VertexFactory和Tessellation的开启.

### BasePassVertexShader
在BasePassVertexShader.usf中, 所做的就是计算BasePassInterpolants和VertexFactoryInterpolants的值. 而这些计算过程就有点复杂了. 有许多的特殊情况, 由preprocessor定义 选择声明不同的interpolators, 决定给哪些属性赋值.

例如, 在BasePassVertexShader.usf中, 利用#if WRITES_VELOCITY_TO_GBUFFER , 根据这一帧和上一帧当前顶点世界坐标的差值计算出这个顶点的速度, 并存储在BasePassInterpolants变量中. 这意味着仅仅需要把Velocity写到GBuffer的Shader变体才会执行这个计算, 这减少了Shader stage之间的数据传输, 和计算量.

### Base Pass Pixel Shader



#### Material Graph to HLSL
当我们在Editor 中创建Material Graph时, UE4会把这些Node Network翻译成HLSL代码. 这些代码会被编译器插入到HLSL Shader中.在MaterialTemplate.ush中包含了许多没有内容的结构体, 仅仅有个%s. 把这作为字符串格式化的标记, 用Material graph生成的内容替换它.

不仅限于结构体, 很多函数的函数体也是个%s. half GetMaterialCustomData0, half3 GetMaterialBaseColor, half3 GetMaterialNormal, 等等， 这些函数的函数体会根据Material graph的内容填充.这样就可以从PixelShader中调用这些函数, 最终调用到MaterialGraph中的节点, 并返回当前Pixel的结果.

#### The "Primitive" Variable
名为`Primitive`的变量, 在Shader代码中全局搜索并找不到它的定义, 因为它是在C++端通过一些宏声明的. 这个宏声明了一个结构体, 在每个Primitive在GPU端绘制之前渲染器就会设置它的值.他的声明在`PrimitiveUniformShaderParameters.h`顶端,可以看到它的成员都有些啥.

#### Creating the GBuffer(Geometry Buffer)

GBuffer是一系列的渲染结果, 保存场景的Geometry信息, 像World Normal, Base Color, Roughness 等等. 用这些信息和光照计算最终的着色.

GBuffer的具体内容是可配置的, 它的通道数和用处可以具体的设置决定.通常有5个GBuffer Textures, 从A~E:
```
GBufferA.rgb = World Normal
GBufferA.alpha = PerObjectGBufferData
GBufferB.rhba = Metallic, Specular, Roughness, ShadingModelID
GBufferC.rgb = BaseColor
GBufferC.alpha = GBufferAO
GBufferD = Custom data
GBufferE = Precomputed shadow factors
```

BasePassPixelShader.usf中的FPixelShaderInOut_MainPS函数是PixelShader的入口. 根据Lighting Model 和 开启的feature的不同, 计算GBuffer需要的数据. 这个函数最开始部分, 就是调用Material graph中实现的函数, 把得到的结果保存下来, 包括 BaseColor, Metallic, Specular, MaterialAO,  Roughness等.
```c++
half3 BaseColor = GetMaterialBaseColor(PixelMaterialInputs);
half  Metallic = GetMaterialMetallic(PixelMaterialInputs);
half  Specular = GetMaterialSpecular(PixelMaterialInputs);

float MaterialAO = GetMaterialAmbientOcclusion(PixelMaterialInputs);
float Roughness = GetMaterialRoughness(PixelMaterialInputs);
float Anisotropy = GetMaterialAnisotropy(PixelMaterialInputs);
uint ShadingModel = GetMaterialShadingModel(PixelMaterialInputs);

half Opacity = GetMaterialOpacity(PixelMaterialInputs);
```
后面再根据具体的着色模型的不同对这些数据作进一步的处理. 例如, 当着色模型用了次表面散射时, 将会调用GetMaterialSubsurfaceData  计算次表面的颜色， 如果没有则为默认值0.

在计算次表面颜色后, 开始计算`DBuffer Decals`对GBuffer的修改.

往后再计算Opacity, 基于从Material graph获得的结果, 作一些volumetric lightmap calculations.

最终创建FGBufferData结构体, 把代表当前像素的GBuffer 数据的值写进去.


#### Setting the GBuffer Shading Model
接下来, 调用定义在`ShadingModelsMaterial.ush`中的函数`SetGBufferForShadingModel`,让每个着色模型以自己的方式修改GBuffer. 通常都不会有什么修改, 一些着色模型会向GBuffer中的自定义数据通道添加额外的数据. 这个函数还会把ShadingModelID写到GBuffer中, 它只是一个整数值, 可以让DeferredPass直到这个Pixel是用的什么着色模型.


如果想使用GBuffer中的自定义数据通道, 需要修改BasePassCommon.ush中的一个预处理宏 WRITES_CUSTOMDATA_TO_GBUFFER, 把自己定义的ShadingModel加上去, 不然自定义的数值会被消除.

#### Using the Data
现在, 每个LightingModel都可以选择把自己需要的数据写到FGBufferData结构体中, BasePassPixelShader中计算了通用的一些数据, 速度, 次表面颜色, 重写Roughness 等.

然后, 把预计算的间接光照和天光数据(GetPrecomputedIndirectLightingAndSkyLight)添加到GBuffer的DiffuseColor中. 接下来还有Translucent, Vertex fogging, debugging 相关的代码, 最后, 调用EncodeGBuffer(DeferredShadingCommon.ush), 把FGBufferData输出到GBuffer纹理中.

### Review
BasePassPixelShader负责通过调用从MaterialGraph生成的函数采样多个PBR数据通道, 并写到一个FGBufferData结构体中, 后续基于不同的着色模型对这些数据进一步处理, 同时还会把ShadingModelID写进纹理, 这样后续就知道当前像素用的是什么着色模型. 最终FGBufferData中的数据被编码到多个RenderTarget中, 给后续的Pass使用.

### Deferred Light Pixel Shader
在DeferredLightPixelShaders.usf中, 会计算每一个Light对一个像素的影响, 为了减少计算量, 会用一个简单的VertexShader画出每个Light可能的影响范围, 例如点光源就是一个球形的范围, Spot Light就是一个锥形的范围.这相当于创建了一个Mask, 仅被这个Mask覆盖到Pxiel才需要计算光照.

#### Shadowed and Unshadowed Lights
