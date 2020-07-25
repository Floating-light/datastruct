# 编辑器Content Browser资源右键菜单扩展
在编辑器的`Content Browser`中，单选或多选资源并右键单击之后会出现一个菜单，里面有各种对这种资源(或通用的)的操作，我们也可以为指定的资源添加自定义的操作。这里以编辑器插件`Paper2DEditor`中通过`Texture`创建`Sprite`的操作扩展为例学习。

在`FContentBrowserModule`模块中`(ContentBrowserModule.h)`，有许多和编辑器`Content Browser`相关的委托数组:
```c++
// Source\Editor\ContentBrowser\Public\ContentBrowserDelegates.h
DECLARE_DELEGATE_RetVal(TSharedRef<FExtender>, FContentBrowserMenuExtender);
DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<FExtender>, FContentBrowserMenuExtender_SelectedAssets, const TArray<FAssetData>& /*SelectedAssets*/);
DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<FExtender>, FContentBrowserMenuExtender_SelectedPaths, const TArray<FString>& /*SelectedPaths*/);

// Source\Editor\ContentBrowser\Public\ContentBrowserModule.h
/** All extender delegates for the content browser menus */
TArray<FContentBrowserMenuExtender_SelectedPaths> AssetContextMenuExtenders;
TArray<FContentBrowserMenuExtender_SelectedPaths> PathViewContextMenuExtenders;
TArray<FContentBrowserMenuExtender> CollectionListContextMenuExtenders;
TArray<FContentBrowserMenuExtender> CollectionViewContextMenuExtenders;
TArray<FContentBrowserMenuExtender_SelectedAssets> AssetViewContextMenuExtenders;
TArray<FContentBrowserMenuExtender> AssetViewViewMenuExtenders;
TArray<FContentBrowserCommandExtender> ContentBrowserCommandExtenders;
```
当我们右键单击选中的资源时,就需要创建一个菜单列表, 此时就会遍历这些数组中的委托,根据这些委托返回的FExtender创建出菜单中的一个个条目。其中，FExtender表示不同类型的编辑器扩展menu items，pull-down menus, toolbar items等，其维护了一个成员:
```c++
TArray< TSharedPtr< const FExtensionBase > > Extensions;
```
表示它的所有扩展。它可以添加不同类型的扩展，也可以把这些扩展应用到不同的UI元素类型
```c++
// AddExtension
SLATE_API TSharedRef< const FExtensionBase > AddMenuBarExtension( FName ExtensionHook, EExtensionHook::Position HookPosition, const TSharedPtr< FUICommandList >& CommandList, const FMenuBarExtensionDelegate& MenuBarExtensionDelegate );

SLATE_API TSharedRef< const FExtensionBase > AddMenuExtension( FName ExtensionHook, EExtensionHook::Position HookPosition, const TSharedPtr<FUICommandList >& CommandList, const FMenuExtensionDelegate& MenuExtensionDelegate );

SLATE_API TSharedRef< const FExtensionBase > AddToolBarExtension( FName ExtensionHook, EExtensionHook::Position HookPosition, constTSharedPtr< FUICommand
// Apply
SLATE_API void Apply( FName ExtensionHook, EExtensionHook::Position HookPosition, FMenuBarBuilder& MenuBarBuilder ) const;

SLATE_API void Apply( FName ExtensionHook, EExtensionHook::Position HookPosition, FMenuBuilder& MenuBuilder ) const;
	
SLATE_API void Apply( FName ExtensionHook, EExtensionHook::Position HookPosition, FToolBarBuilder& ToolBarBuilder ) const;
```

```c++
TSharedRef< const FExtensionBase > FExtender::AddMenuExtension( FName ExtensionHook, EExtensionHook::Position HookPosition, const TSharedPtr< FUICommandList >& CommandList, const FMenuExtensionDelegate& MenuExtensionDelegate )
{
	TSharedRef< FMenuExtension > MenuExtension( new FMenuExtension );
	MenuExtension->Hook = ExtensionHook;
	MenuExtension->HookPosition = HookPosition;
	MenuExtension->CommandList = CommandList;
	MenuExtension->MenuExtensionDelegate = MenuExtensionDelegate;
	Extensions.Add( MenuExtension );

	return MenuExtension;
}
```


其扩展的入口很简单:
在`ContentBrowserExtensions.h`中仅有:
```c++
class FPaperContentBrowserExtensions
{
public:
	static void InstallHooks();
	static void RemoveHooks();
};
```
其中`InstallHooks()`实现了,
