# 编辑器Content Browser资源右键菜单扩展
在编辑器的`Content Browser`中，单选或多选资源并右键单击之后会出现一个菜单，里面有各种对这种资源(或通用的)的操作，我们也可以为指定的资源添加自定义的操作。这里以编辑器插件`Paper2DEditor`中通过`Texture`创建`Sprite`的操作扩展为例学习。

## FContentBrowserModule
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
当我们右键单击选中的资源时,就需要创建一个菜单列表, 此时就会遍历这些数组(`AssetViewContextMenuExtenders`)中的委托,根据这些委托返回的FExtender创建出菜单中的一个个条目。

## FExtender
FExtender表示不同类型的编辑器扩展menu items，pull-down menus, toolbar items等，其维护了一个成员:
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
显然，右键菜单是应该扩展菜单, 即调用`FExtender::AddMenuExtension()`.

## 编辑器ContentBrowser右键菜单扩展
其中,前面两个参数是指明扩展的位置,最后一个参数是应用扩展时的回调函数(委托):
```c++
/** Called on your extension to add new menu items and sub-menus to a pull-down menu or context menu */
DECLARE_DELEGATE_OneParam( FMenuExtensionDelegate, class FMenuBuilder& );
```
他会传入需要被扩展的MenuBuilder, 我们应该在这个委托中对这个Menu作我们想要的扩展。
```c++
static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender(new FExtender());
    // ... ...
    // 根据选中的资源,判断是否要创建这一扩展
	// Add the sprite actions sub-menu extender
	Extender->AddMenuExtension(
		"GetAssetActions",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateStatic(&FPaperContentBrowserExtensions_Impl::CreateSpriteActionsSubMenu, SelectedAssets));
    // ... ...

	return Extender;
}
```
其中`TBaseDelegate::CreateStatic`从一个原始c++函数指针创建委托,是一个模板函数, 并将`CreateSpriteActionsSubMenu`的最后一个参数绑定为`SelectedAssets`.
```c++
static void CreateSpriteActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
{
	MenuBuilder.AddSubMenu(
		LOCTEXT("SpriteActionsSubMenuLabel", "Sprite Actions"),
		LOCTEXT("SpriteActionsSubMenuToolTip", "Sprite-related actions for this texture."),
		FNewMenuDelegate::CreateStatic(&FPaperContentBrowserExtensions_Impl::PopulateSpriteActionsMenu, SelectedAssets),
		false,
		FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.PaperSprite")
	);
}
```
至此, Extender就创建好了, 它将返回给调用的地方，直到搜集完所有的扩展，再对所有的Extender调用`FExtender::Apply`, 它会将要扩展的Builder作为参数传给它添加扩展时传入的那一委托,并调用,即上面的`CreateSpriteActionsSubMenu`,其中创建了一个子菜单，所以它还会有一列子菜单Entry从它旁边展开,它会调用第三个参数绑定的函数`PopulateSpriteActionsMenu`来创建:
```c++
// MultiBoxBuilder.h line: 26
DECLARE_DELEGATE_OneParam( FNewMenuDelegate, class FMenuBuilder& );

// ContentBrowserExtensions.cpp line: 215
	static void PopulateSpriteActionsMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		// Create sprites
        // ... ...
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_CreateSprite", "Create Sprite"),
			LOCTEXT("CB_Extension_Texture_CreateSprite_Tooltip", "Create sprites from selected textures"),
			FSlateIcon(PaperStyleSetName, "AssetActions.CreateSprite"),
			Action_CreateSpritesFromTextures,
			NAME_None,
			EUserInterfaceActionType::Button);

		// Extract Sprites
        // ... ...
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_ExtractSprites", "Extract Sprites"),
			LOCTEXT("CB_Extension_Texture_ExtractSprites_Tooltip", "Extract sprites from selected textures"),
			FSlateIcon(PaperStyleSetName, "AssetActions.ExtractSprites"),
			Action_ExtractSpritesFromTextures,
			NAME_None,
			EUserInterfaceActionType::Button);

		// Configure the selected textures according to the project settings (same as if it got imported from a sprite sheet)
        // ... ...
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_ConfigureTextureForSprites", "Apply Paper2D Texture Settings"),
			LOCTEXT("CB_Extension_Texture_ConfigureTextureForSprites_Tooltip", "Sets compression settings and sampling modes to the defaults specified in the 'Paper2D - Import' project settings"),
			FSlateIcon(PaperStyleSetName, "AssetActions.ConfigureForRetroSprites"),
			Action_ConfigureTexturesForSprites,
			NAME_None,
			EUserInterfaceActionType::Button);

		// Create sprites
        // ... ...
		MenuBuilder.AddMenuEntry(
			LOCTEXT("CB_Extension_Texture_CreateTileSet", "Create Tile Set"),
			LOCTEXT("CB_Extension_Texture_CreateTileSet_Tooltip", "Create tile set from selected texture"),
			FSlateIcon(PaperStyleSetName, "AssetActions.CreateTileSet"),
			Action_CreateTileSetFromTextures,
			NAME_None,
			EUserInterfaceActionType::Button);
	}
```
这里添加了四个子菜单Entry, 参数`EUserInterfaceActionType::Button`表明它像按钮一样，会在按下时调用第四个参数`FUIAction`绑定的一个回调函数,没有参数的委托。

## 注册
以上,整个扩展的流程基本就完成了, 还有最后一步, 将扩展的回调的函数委托`OnExtendContentBrowserAssetSelectionMenu`添加到`AssetViewContextMenuExtenders`数组中, 以便在编辑器右键单击时得到要扩展的`FExtender`.
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
其中`InstallHooks()`实现了注册扩展委托:
```c++
void FPaperContentBrowserExtensions::InstallHooks()
{
	ContentBrowserExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FPaperContentBrowserExtensions_Impl::OnExtendContentBrowserAssetSelectionMenu);
    // 获取FContentBrowserModule中的这个数组
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	// 添加这一委托.
    CBMenuExtenderDelegates.Add(ContentBrowserExtenderDelegate);
	ContentBrowserExtenderDelegateHandle = CBMenuExtenderDelegates.Last().GetHandle();
}
```
`RemoveHooks`也仅仅是简单地从这个数组中找到注册的那个委托，从中移除掉.

而`InstallHooks()`是在`FPaper2DEditor`模块的初始化函数`OnPostEngineInit()`中调用的.而这个函数在`StartupModule()`中被绑定到一个全局的委托`FCoreDelegates::OnPostEngineInit`中,它会在引擎初始化的时候被调用:
![初始化](./init.png)
