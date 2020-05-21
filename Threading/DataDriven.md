## Why？——DataDirven
#### 职责分离
* 分工协作，策划配表，程序读取配置好的数据。
* 易维护，同类或相关联的数据都可在同一地方找到。不要散落在代码的各个地方。
* 外部数据中心，sql数据库。
* 易于调试，可轻松切换为调试数据。
* 易于迭代，更改数据方便。
#### 少硬编码数据
* 少在c++代码中出现硬编码数据。
* 蓝图的Class Default 的Details面板中也最好不要用，有时容易出现意想不到的效果。

#### 少硬编码类
蓝图中直接SpawnActor写死，用基类的类引用。
#### 自动化数据配置流程
利用EditorUtilities进行编辑器扩展

利用C++进行额外的接口暴露和扩展
## 数据配置方式
#### 初级：
    BP CDO/Instance
    C++ ConstructHelpers
    Class Reference
#### 中级：
    DataTable/CompositeDataTable
    DataAsset/PrimaryDataAsset
    CurveTable/CompositeCurveTables
    Custom WorldSettings
#### 高级：
    Settings(.ini)
    ConsoleVariables
    Raw：txt/csv/json/xml
    Enhanced: sql/excel
### 工具：
    BulkEdit
    EditorUtility

### 1. BP CDO(Class Default Object/Instance)
在BP中用结构数组来保存数据，并在蓝图中动态遍历这些数据以动态生成UI或Actor。
但是数据会散落在项目蓝图的各个角落，难以定位更改，不方便维护和迭代。
### 5.DataAsset/PrimaryDataAsset

### n. 控制台变量
通常我们可以通过 \` 调出控制台命令行，通过它可以改变一些参数的值。这些参数的定义一般是通过`TAutoConsoleVariable`模板类实现。
我们可以在一个文件中定义如下的静态变量：
```c++
// #include "HAL/IConsoleManager.h"

static TAutoConsoleVariable<bool> ConsoleEnabled(
	TEXT("im.testBool"),
	false,
	TEXT("The test console variable-->bool"),
	ECVF_Default);

static TAutoConsoleVariable<int> ConsoleCount(
	TEXT("im.testInt"),
	233,
	TEXT("The test console variable-->int"),
	ECVF_Default);

static TAutoConsoleVariable<FString> ConsoleName(
	TEXT("im.testString"),
	TEXT("defaultValue"),
	TEXT("string ---> help information"),
	ECVF_Default);
```
`TAutoConsoleVariable`模板类支持`bool`，`int`,`FString`三种类型的控制台变量，然后就可以在控制台通过指定的`Name`更改对应的变量值。

在程序中访问变量的值用：
```c++
FString str = FString::Printf(TEXT("im.testBool: %s"), ConsoleEnabled.GetValueOnGameThread() ? TEXT("true") : TEXT("false"));

str += FString::Printf(TEXT("\nim.testCount: %d"), ConsoleCount.GetValueOnGameThread());

str += FString::Printf(TEXT("\nim.testName: %s"), *ConsoleName.GetValueOnGameThread());
```
