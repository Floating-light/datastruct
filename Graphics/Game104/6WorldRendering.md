## 1. Terrain
### 1.1 Adaptive mesh tessellation
在视锥下，越近网格越密集。依赖于视角。
* View到Camera的距离和FOV
* 在动态细分后，屏幕上的误差不能超过一个像素

#### 1.1.1 如何细分Mesh - 三角形
* 基于三角形（等腰直角三角形）， 总是从直角边到直角顶点细分。
* T-Junction

Cons:

对地形数据的管理和细分算法不符合直觉（三角形）。
#### 1.1.2 如何细分Mesh - QuadTree-Base subdivision
* 容易构建
* 易于管理数据，包括对象剔除和数据流送，纹理， 各种贴图 （分块）。

* Mesh细分不如三角形灵活
* 网格层面的叶节点需要时连续的

解决T-Junction：
在连接的交界，更细的一遍把中间点往两边移动，-- 退化三角形。使交界处细分程度一致。

#### 1.1.3 如何细分Mesh - 不规则三角形
根据区域的平坦程度，越平坦，三角形越稀疏，通过预处理实现.
* 实时渲染很简单
* 在特定地形下可以只使用更少的三角形

* 需要预处理
* 不好复用

#### 1.1.4 GPU-细分

DX11 Hull Shader，Tessellator Shader， Domain Shader。
DX12 Mesh Shader.

## 1.2 地形中的洞

projectedPosition /= isHole ? 0 : 1 

## 1.3 体素地形
洞, 各种奇怪的结构, 而不局限于平坦地面.

Marching Cubes.

## Floating point precision error 

## Tree Rendering
SpeedTree
## Decorator rendering 

## Road and Decals Rendering 

## 大气散射

a scalable and production ready sky and atmosphere rendering technique

## Cloud

### Mesh-Based Cloud Modeling

### Billboard Cloud 

### Volumetric cloud modeling 

