## Uniforms

从CPU 向GPU 传递数据.
* Global. per shader program. 
* keep value until reset or update.

```c++
uniform vec4 ourColor;
```
可以在任何shader stage 中定义, 一个program 中的都是一样的.

如果声明了一个uniform ,却没有使用, 编译器会悄悄移除这个变量.

从CPU中改变一个Shader Program中的uniform变量的值:
1. 找到Program中uniform变量的index/location. 
int uniformVarID = glGetUniformLocation(shaderProgram, "UniformVarName");
如果没找到则返回-1;
2. update values. ---> glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);

## 7.7 Texture Units 纹理单元
```
uniform sampler2D texture1;
```
如何与纹理关联的? 也没有用glUniform设置它的值? 但是可以设置一个Texture location给这个变量, GL会根据这个位置将对应sampler与这个texture绑定.
这个位置叫纹理单元(Texture Units). 因此, 在一个shader program中可以使用多个texture.

我们的每Generate一个texture, 都要放到一个纹理单元上, 一般默认在0. 
```GLSL
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);
```
在激活一个texture unite 之后的 glBindTexture会同时也将这个texture绑定到对应的纹理单元.或者说glBindTexture就是将texture绑定到当前激活的纹理单元.
默认会激活第0个texture units.

OpenGl至少应该有16个texture unit 可以用, 

## 48 Text Rendering
128 个基本的相同大小的字符是很easy的.但是:
* 每个字符有不同的width, height , margin.
* 超出128个基本字符
* 特殊符号, 数学表达式, 音符.
* 从上往下绘制.
这些操作都很复杂, 因为它并不属于像OpenGL这样的底层API.所以我们必须自己定义一套这样的系统.
* 通过GL_LINES画.
* 创建字符的3D meshes.
* 将字符texture渲染到3D场景(environment)中的四边形上.
大多数developers选择将字符textures 渲染到quads上, 这本身不难, 但把字符弄到texture上却不简单.

以下讨论几种方法, 并用FreeType实现一个更先进更灵活的渲染text的技术.

### 48.1 bitmap fonts
把一个font中的所有字符都放到一张texture上, 叫bitmap font.
这些一个个符号叫做glyphs.渲染时把一个由texture coordinates 指定的字符渲染到一个2D quad 中。
bitmap font 可由Codehead's Bitmap Font Generator(www.codehead.co.uk/cbfg/)生成.

advantages:
* easy to implement
* pre-rasterized, quite efficient.

disadvantages:
* not flexible for different font.
* resolution limited, zooming .
* limited to a small character set.

### 48.2 Modern text rendering: FreeType

FreeType库: 
* load fonts
* render them to bitmaps
* 提供font相关的操作.
* load TrueType fonts
`TrueType` 用样条线, 即一系列控制点来表示的字体.可以生成任意分辨率大小的图形而不会有任何质量上的损失.

```
https://www.freetype.org/download.html
Windows 已经编译好的库: https://github.com/ubawurinna/freetype-windows-binaries
```
FreeType 加载TrueType fonts, 对每一个glyph, 都生成一个bitmap image和一些matrics.提取这些bitmap images 生成textures, 并用对应的metrics把每个字符放在恰当的位置.

