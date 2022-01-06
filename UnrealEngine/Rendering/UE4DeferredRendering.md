# Deferred Render

核心是以两个Pass渲染场景:
1. Geometry Pass, 得到Geometry Buffer。
2. Lighting Pass, 对每一个Pixel计算光照。
基于屏幕空间的技术. 将计算量大的光照延后，且和物体数量解耦，使光照计算次数仅与分辨率有关。

* Deferred Lighting(Light Pre-Pass)
* Tiled-Based Deferred Rednering
* Clustered Deferred Rendering
* Decoupled Deferred Shading 
* Visibility Buffer
* Fine Pruned Tiled LightPass 
* Deferred Attribute Interpolation Shading
* Deferred Coarse Pixel Shading
* Deferred Adaptive Compute Shading
