## target_source()
之前的方法, 用一个变量保存所有source, 并通过include cmakefile, 让这些file自己把自己的source加进去, 最后在最Top的CmakeLists.txt中add_library或add_execute()编译.

drawback:

* 文件的数量和名称不可控
* 子目录无法配置target属性(那时Target还没定义)
* 只能定义更多的变量把配置传上来
* 一个Target就定义这么多变量, 很蠢.

```c++
target_sources(myLib
    PRIVATE
        foo.cpp
        foo_p.cpp
        foo_p.h
    PUBLIC
        foo.h  # poor PUBLIC example, see discussion below for why
)

find_library(BARRY_LIB barry)
# This call requires CMake 3.13 or later, see next section
target_link_libraries(myLib PUBLIC ${BARRY_LIB})

target_compile_definitions(myLib PUBLIC USE_BARRY)
target_include_directories(myLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
```

将.h也添加到source并不会让它们被直接编译, 但是会让IDE generator受益, 即使没有被Include, 也可以让这些头文件被列入这些工程的文件列表内. 在开发时更容易发现并有利于重构.

PRIVATE和PUBLIC指定Source被使用的地方:

PRIVATE: 仅仅在myLib编译时使用.
PUBLIC: myLib和所有所有Link到myLib的target.
INTERFACE: 仅仅Link到myLib的Target.

在实践中, sources几乎总是Private的, 因为它们通常都不应该添加到任何Link它们的Target中.Header--Only的库是一个例外, 对于Interface库, 可以只把Sources作为INTERFACE添加.千万不要把PRIVATE,PUBLIC和INTERFACE和库的头文件是否是public API 混淆. 这里, 这些关键字只控制Sources应该被添加到哪些Target的编译中.

还有一些不太常见的情况, 一些文件(resources, images, data files)可能需要直接被编译到Link到 在Runtime找到这些资源的库 的Target中.把这些资源作为PUBLIC或INTERFACE对这种情况很有帮助. Note: 安装非私有的source可能会出现问题.

其它的target_...()的命令也有相似的用法,只是设为non-private更常见.上面的例子中, 所有link到myLib的Target都会link到barry,并且添加定义 USE_BARRY.target_include_directories给myLib添加了header的搜索路径, 以及所有link到myLib的target也会添加这一路径.其它的Taget的代码中inlcude了myLib中的header也能够找到.

https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/

## add_subdirectory 结构化工程
```
add_library()
target_sources()
target_include_directories()
target_link_libraries()
```
limits: 不允许链接到当前目录之外的库
solve: 用 module.

build tree 模仿source tree的结构: 
```c++
message("current binary dir is ${CMAKE_CURRENT_BINARY_DIR}")
```

限制build目录不能和source目录一样:
```c++
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()
```

3.12: OBJECT libraries:

```
add_library(io OBJECT "")
```
### 绘制依赖图
build>
cmake --graphviz=example.dot ..
http://www.graphviz.org/download/#windows
dot -T png example.dot -o example.png

## 使用Module include, link定义在当前目录之外的Target
直接include CMakeLists.txt
```c++
include(src/CMakeLists.txt)
include(external/CMakeLists.txt)
```
这样在`src/CMakeLists.txt`中就可以link到`external/CMakeLists.txt`中的库.如果用add_subdirectory这是不可能的.

