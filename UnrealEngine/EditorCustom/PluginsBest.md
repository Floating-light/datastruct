# Asset Types Overview

Common Tasks:
* 声明资产类型的C++类, 通常继承自UObject.
* 实现用户创建资产实例的方法, 即Asset Factories.
* 自定义资产在编辑器中的外观, 快捷菜单、缩略图颜色和图标、Detail 面板自定义(蓝图编辑器,材质编辑器)、过滤、分类等.
* 特定资产的ContentBrowser actions.即右键菜单.
* 对复杂Asset类型,自定义Asset编辑器UI.

# Asset Factories

Factory Types
* Content Browser Context Menu,右键菜单, {类型名}FactoryNew.
* Content Browser Drag & Drop, {TypeName}Factory.
* Automatic Reimport,当硬盘上文件改变时重新创建资产, Reimport{TypeName}Factory*

UFactory
* 所有资产工厂的基类
* 编辑器集成的核心逻辑
* 重写虚函数
* 有很古老的API :(

* 在构造函数中通过一些bool值的控制来改变工厂的类型为右键创建, 拖拽创建或自动重新导入.
```c++
// Factory API 至少15年历史, 有点过时

// 编辑器右键创建
UTextAssetFactoryNew::UTextAssetFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UTextAsset::StaticClass(); // 支持的类型
	bCreateNew = true; // 创建新实例, 而不是拖放资源
	bEditAfterNew = true;  // 创建资源后编辑名字
	// 这些bool也是 必须为每种工厂使用不同工厂类的原因
	// Why not 用一个工厂实现右键创建，拖放创建，自动重新导入
}

UTextAssetFactory::UTextAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	// 告诉工厂支持的拖入的文件格式
	Formats.Add(FString(TEXT("txt;")) + NSLOCTEXT("UTextAssetFactory", "FormatTxt", "Text File").ToString());
	SupportedClass = UTextAsset::StaticClass();
	bCreateNew = false; // 指定为拖放时调用
	bEditorImport = true; // 从外部文件导入
}
```
* 继承自UFactory，编辑器自动发现并注册它们

* 4.25 必须同时实现FAssetTypeActions_Base才能在右键菜单看到新建按钮
https://answers.unrealengine.com/questions/969157/ue425-factorycreatenew-dont-work.html
4.24及之前则没有这个问题

* 打包出现找不一些插件或其中模块中的类的定义时, 尝试将该模块的"LoadingPhase"改为“PreDefault”, (在该模块归属的插件的uplugin文件中设置)
```                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
ogInit: Display: LogBlueprint: Error: [Compiler ThirdPersonCharacter] In use pin  NewVar_0  no longer exists on node  Get . Please refresh node or break links to remove pin. from Source: /Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter
UATHelper: Packaging (Windows (64-bit)):   LogInit: Display: LogBlueprint: Error: [Compiler ThirdPersonCharacter] The property associated with  Text  could not be found in '/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter_C' from Source: /Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter
UATHelper: Packaging (Windows (64-bit)):   LogInit: Display: LogUObjectGlobals: Warning: While loading '../../../../../../Myfloder/UnrealProject/MyProject/Content/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.uasset' failed to load '/Script/TextAsset': Can't find file.
```

