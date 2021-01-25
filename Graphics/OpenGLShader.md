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
unsigned int uniformVarID = glGetUniformLocation(shaderProgram, "UniformVarName");
如果没找到则返回-1;
2. update values. ---> glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
