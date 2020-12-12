# 启动exe无响应
不能在c++ 中定义全局的静态变量。

# 导入贴图不允许非2的幂大小的图片?
如果当前路径下有连续编号的图片会自动识别为UDIM贴图,此贴图要求大小必须为2的幂.即使仅导入一张也会作这种判断。

### ActorComponent
如果需要直接在`Add Component`的列表中出现,需要在类的声明前额外加上:
```c++
meta=(BlueprintSpawnableComponent)
```
# All thing entry: https://www.zhihu.com/question/356772225/answer/906182193
# Engine framework - Introduce and theory
1. some.uproject
some.uproject中包含
"EngineAssociation": "4.25",
字段,表示Eingine的Build ID。标准引擎版本一般是"4.25"之类的。当通过源码编译时，他会自动生成一个或手动指定，并将其写入注册表。这样就可以找到本机有的不同版本的引擎。

- 避免更换版本，避免改动引擎。

- Naming Conventions and Standards
https://github.com/Allar/ue4-style-guide

2. 委托参数绑定
- ContentBrowserExtensions.cpp, Line 222
通过原始c++函数指针创建全局函数委托
```c++
TBaseDelegate::CreateStatic(&FPaperContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(SpriteCreatorFunctor));
```
- 参数绑定:
```c++
// 函数原型:
static void CreateSpriteActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets);
// 所有类型委托的基类
// 将第二个参数绑定为一个本地的变量.
TBaseDelegate::CreateStatic(&FPaperContentBrowserExtensions_Impl::CreateSpriteActionsSubMenu, SelectedAssets));
```
3. NewEditorMode: https://github.com/AlexanderSarton/NewEditorMode

4. 蓝图模板函数?不可能!用meta. https://answers.unrealengine.com/questions/742529/templatewildcard-return-type-for-blueprints.html
- Developers floder 
烘培时会忽略

5. UBT如何使用C#中的Build内容：https://docs.unrealengine.com/en-US/Programming/BuildTools/UnrealBuildTool/ModuleFiles/index.html
- UE_LOG
https://www.cnblogs.com/blueroses/p/6037981.html

6. TMap, TMultiMap
- 对Key进行散列, Key必须支持GetTypeHash() 和 `==`
- 任选分配器,`TSetAllocator`.(而不是`FHeapAllocator`和`TInlineAllocator`).指定映射应使用的散列桶的数量以及应使用哪一个标准UE4分配器来存储散列和元素。
- KeyFuncs 最后一个TMap模板参数, 告诉映射如何从元素类型获取Key,如何比较两个Key是否相等,以及如何对Key进行散列计算


- 字符编码问题
在出现中文literal时,由于文本的编码错误，会导致编译器(或者是UHT)识别不了或者识别成其它字符，然后报一些莫名其妙、令人摸不着头脑的错，此时可以将文档保存为utf-8编码即可解决。
https://blog.csdn.net/weixin_42109012/article/details/957475763. 

- TArray
按指定字符串分割,返回分割后的字符串组成的数组
dir.ParseIntoArray(arr, TEXT("/")); 

- FPaths
FPaths::Combine(LabelXmlDirectory, TEXT("placemarklayout.xml"));
FPaths::NormalizeDirectoryName(dir);
FPaths::FileExists(realPath)
FPaths::DirectoryExists(dirs)

- FString
https://docs.unrealengine.com/zh-CN/Programming/UnrealArchitecture/StringHandling/index.html

# 获取烘培纹理
// D:\Program Files\Epic Games\UE_4.25\Engine\Plugins\2D\Paper2D\Source\Paper2D\Classes\PaperSprite.h
// line: 296
//	UTexture2D* GetBakedTexture() const;

# 实例化各种东西

com = NewObject<UStaticMeshComponent>(this, TEXT("ffff"));
	AddInstanceComponent(com);

# GC 错误
```
The  Game has crashed and will close
```
多半是没加UPEOPERTY(), 在所有引用了UObject对象的属性上都要加UPROPERTY().指针一定要初始化为nullptr.

# Project Config modify in runtime
 https://answers.unrealengine.com/questions/606337/config-not-being-saved-uproperty-uclass.html
 
# 序列化
https://www.jianshu.com/p/9fea500aaa4d