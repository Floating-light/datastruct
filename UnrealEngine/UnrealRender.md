# Deferred redering
1. 着色(Shading)延迟发生
2. 用GBuffer组合work
3. 对渲染动态光照友好
4. 性能稳定可预测
5. 关闭一些特性时很灵活, 但对表面属性不灵活
6. 不能用MSAA(Mutisample anti-aliasing), 用TAA(Temporal anti-aliasing)

# Forward redering
1. 几何/材质 的的渲染和着色同时发生
2. 光照和材质的计算更加灵活, 但在混合多个特性时不灵活。
3. 擅长半透明表面的渲染.
4. 简单的应用更快
5. 动态光照对性能的影响很大
6. 可以用MSAA, 抗锯齿效果更好

## 2. 渲染之前
CPU上有一个普通thread和一个DRAW thread.GPU Thread 和CPUthread同步.

### 2.1 Frame 0 - Time 0 - CPU
计算所有的逻辑和变换(GameThread)
* Animations
* Positions of models and objects.
* Physics
* AI
* Spawn and destroy, Hide and Unhide 

### 2.2 遮挡 Frame 1 - Time 33ms - Draw Thread (mostly CPU)

什么是可见的?

构建一个可视性列表, 逐对象进行.
1. Distance Culling (距离剔除)
物体离相机的距离超过某个值就不渲染.
默认关闭.
在所有可见物体的Details 面板下的render分类下, 可设置剔除距离.
也可以用剔除体积, CullDistanceVolume, 其中的物体都会收到影响.
2. Frustum Culling (视锥剔除)
不在相机的可视范围内的物体就不渲染, 几乎无法关闭.
3. Precomputed Visibility (预计算可见性)
首先在worldsetting中开启, 
![SetupPrecomputeVisibility](./image/SetupPrecomputeVisibility.png)
创建一个PrecomputedVisibilityVolume, 包住想要预计算的体积.
构建光照。
![VisualizePrecomputed](./image/VisualizePrecomputed.png)
然后就可以看见预计算的cell.

4. Occlusion Culling (遮挡剔除)

性能消耗大.
freezerendering, stat Initviews
遮挡的性能影响:
1. 总是开启距离剔除, 没有先剔除的在计算遮挡时的消耗很大.
2. 超过10-15K的对象影响很大, 基准测试, 概要分析.
3. 大型开放式场景无法很好地应用遮挡
4. 粒子(Bounding Box)的遮挡, 任何可见对象都能被遮挡
5. 大型物体很少被遮挡, 减少了CPU的遮挡计算,但增加GPU渲染消耗
6. 组合小模型到一个大的模型, 增加CPU的消耗(遮挡计算) 


# 几何结构渲染
## Prepass / Early Zpass

逐对象渲染.

互相部分遮挡的物体导致一些像素重复被多次渲染.预渲染Z Pass, 确定每个对象的那一部分最终被显示在屏幕上.

## Draw calls
共享同一属性的一组多边形就是一个drawcall, 逐对象(材质)调用.相同材质的对象逐个接连调用.

stat RHI 

Draw Primitive calls 就是drawcall 调用次数

2000 —— 3000 是比较合理的(reasonable)
超过5000 就有点高了
超过10000就可能会成为一个问题
在移动设备上更低(最多几百)

DrawCalls 对性能影响巨大, 每次渲染完都要接收渲染线程的命令, 导致损耗增加. 通常影响比多边形面数大得多.
三角形面数对几何渲染这一步的影响很小.

组件也是组个DrawCall.所以将多个mesh挂到一个actor下与一个个分开没什么区别.

降低DrawCall :
* 使用大型模型而不是小而散的模型, 但大模型太多会导致其它问题(遮挡, 碰撞碰撞计算, 光照贴图 变复杂, 存储空间)
* 模块化网格体, 可以节省工作量和内存, 提升光照, 遮挡, 碰撞, LOD. 但也增加了Draw call.
* 在后期合并网格体.
合并规则:
* 经常被使用且面数也比较低的.
* 仅合并在同一区域内的东西.
* 仅合并有同样材质的东西
* 没有或仅有简单碰撞的Mesh才适合合并.
* 小的Mesh或仅接受动态光照的Mesh也是很好的选择.
* 远距离的几何体

仅当出现问题时再合并, 在非常低端的设备上(移动设备)才需要合并.

模型在内存中实例化, 不会再渲染中实例化, 一个模型放置多个时, 只会在内存中存在一份, 渲染时会渲染多次.

实例化渲染 

Lodding - Level Of Detail

常规LOD 只是减少了面数, 并没有减少Draw call.
HLOD - Hierarchical LOD 在足够远距离时用一个模型替换一组模型.

# Vertex Shaders
有许多不同类型的shader: 顶点着色, 像素着色, whole 着色, 域着色. 

Vertex Shaders 

根据模型的世界坐标, 将顶点本地坐标转换为世界坐标.  处理平滑过渡, 硬边缘, 柔化边缘, 顶点的颜色。
还会处理材质上的偏移, World Position Offset 材质输入. 

World Position Offset 可以干:
1. 布料
2. 水偏移, 波浪
3. 植被波纹
因为不可能都用动画来实现这么多的动画, CPU支撑不了, 但因为着色被超级优化过, 则可以支撑.

顶点着色不会修改模型位置(CPU不会知道这个, 物理, 碰撞也不会影响), 仅仅是渲染的视觉效果上的改变.

性能影响: 
1. 动画越复杂, 越会降低性能》
2. 越多顶点被影响, 越慢
3. 多顶点的对象应该用更简单的顶点着色.
4. 在距离较远时禁用顶点着色便宜, 或者动画之类的不必要的消耗.