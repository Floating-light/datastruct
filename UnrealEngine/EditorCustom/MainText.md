在UE的编辑器中,我们可以通过一些简单的操作在ContentBrowser中创建并编辑一些对象(即资源).
* 在ContentBrowser空白处单击右键创建资源.
* 在ContentBrowser选中一个或多个资源,右键从中创建新的资源.
* 自定义特定资源资源编辑器(双击资源时打开的窗口)

参照`Paper2D`插件, 假设我们需要实现一个播放`Texture`序列的功能, 即将一系列的序列帧图片导入UE4中, 以指定的频率播放它. 这里我们关注的是如何表示这些序列帧图片资源, 通常一段序列有上百张, 一种直观的想法是用一个数组按顺序存下这些图片, 但不会有人想一张张地在蓝图中加到这个数组中！

因此, 我们需要更快捷的方式！

### 1. 创建资产类——UObject
我们可以将这种资源的表示抽象为一个`UTexturePlayer`类, 显然它派生自`UObject`,这样就可以在编辑器中对它进行管理.

```c++
/**
 * A object to play texutre sequence
 */
UCLASS(BlueprintType)
class  YOURMODULENAME_API UTexturePlayer : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class UTexture2D*> SourceTextures;
    
    // other play function ... ...
};
```
### 2. 创建资产对应的工厂——UFactory
UE的`Editor`中, 创建资源通常都是通过`UFactory`, 对每一个希望在编辑器的`ContentBrowser`中创建的资源, 都必须实现一个对应的`UFactory`.

```c++
/**
 * Implements a factory for UTextAsset objects.
 */
UCLASS(hidecategories=Object)
class USpritePlayerFactoryNew
	: public UFactory
{
	GENERATED_UCLASS_BODY()

public:
    // Selected Textures
	TArray<class UTexture2D*> InitTextures;

	//~ UFactory Interface
    // 实现这个之后, 在UE4.24及之前,就会在Create Advanced Asset中的Miscellaneous分类中出现创建按钮,点击即会调用这个函数.
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	/** Returns true if this factory should be shown in the New Asset menu (by default calls CanCreateNew). */
	virtual bool ShouldShowInNewMenu() const override;

	virtual FText GetDisplayName() const override;

};
// 关键部分
UObject* USpritePlayerFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UTexturePlayer* NewTexturePlayer = NewObject<UTexturePlayer>(InParent, InClass, InName, Flags | RF_Transactional);

	NewTexturePlayer->SourceTextures = InitTextures;

	return NewTexturePlayer;
}

```
由于`UFactory`是派生自`UObject`的，所以我们不必手动注册它, 编辑器会自动搜集所有的`Factory`。

### 3. 定义资产在编辑器中的外观——IAssetTypeActions
`IAssetTypeActions`主要定义和这种资产本身相关的东西, 比如缩略图, 颜色, 分类 以及 在`ContentBrowser`中右键单击它的`Action`菜单.同时,引擎实现好了一个符合大多数资产行为的基类`FAssetTypeActions_Base`,我们可以从它派生, 以减少代码量.

```c++
class FTexturePlayerAssetActions
	: public FAssetTypeActions_Base
{
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InType 该资产的自定义分类, 在外面注册再传进来
	 */
	FSpritePlayerAssetActions(EAssetTypeCategories::Type InType, const TSharedRef<ISlateStyle>& InStyle);

public:

	//~ FAssetTypeActions_Base overrides

	virtual bool CanFilter() override { return true; }
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual uint32 GetCategories() override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

private:

	/** Pointer to the style set to use for toolkits. */
	TSharedRef<ISlateStyle> Style;

	EAssetTypeCategories::Type MyAssetType;
};
```