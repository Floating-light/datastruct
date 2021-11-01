https://medium.com/@lordned/unreal-engine-4-rendering-part-5-shader-permutations-2b975e503dd4

# 5.Shader Permutations
Unreal 会编译给定Shader/Material的多个排列以处理不同的使用情况. 当一个Material被修改后, 就会把它用到的.ush/.usf重新加载, 然后把MaterialGraph翻译成HLSL代码, 并构建Shader的每一个排列.

每次使用材质编辑器的时候就会发生这一过程.这里有个问题: 它仅仅会重新加载和这个材质本身相关的Shader.由于Unreal用延迟渲染, 一些Shader是全局的(采样GBuffer,计算最终的颜色). 这意味着这些Shader实际上不是这个material的一部分, 所以修改Material不会导致它们被重新加载.

只要改变了一个GlobalShader, 然后重新启动, Unreal就会重新编译每一个用到这个文件的Shader, 所以如果编辑了一个通用的Shader, 将会几乎重新编译所有东西, 一个空的工程就有125Shaders, 差不多10000个排列.如果修改了Render模块的代码也会这样.这很糟糕, 可能要等上十几分钟.更糟糕的是, 如果修改的是`#if X_Y`里面的代码, 你可能在等待了很长的编译事件后, 才发现一种排列编译不了.等你修改了这一情况后, 又要从头开始编译.

## Lowering the Overall Number of Permutations

首先看看Engine或Material有没有什么设置可以减少编译排列总量的设置. 并不建议在Ship时使用这些设置, 但是在开发Shader时临时启用这些设置来加速迭代时间还是可以的.

在材质上的设置: Usage Setting:

出现在Usage下的设置代表着Engine中指定的Vertex Factories, 如果勾上Automatically Set Usage in Efitor, 当你在新的VF中使用它时就会生成新的排列.关闭这个选项将会使得Artists在Assigning material时思考两次, 而且可能并不会节省太多Permutations.

Quality and Static 选项 ? 

Project Setting > Rendering

... 
### Other 
https://therealmjp.github.io/posts/shader-permutations-part1/
https://blog.csdn.net/QQKeith/article/details/106630734

大佬大佬: https://www.cnblogs.com/timlly/p/15092257.html

Uber shader, shader permutation : https://www.gamedev.net/forums/topic/659145-what-is-a-uber-shader/

NEXT https://medium.com/@lordned/ue4-rendering-part-6-adding-a-new-shading-model-e2972b40d72d
