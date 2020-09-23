```c++
/**
 * Renames a single object
 *
 * @param Object								The object to rename
 * @param PGN									The new package, group, and name of the object.{/Game/NewSprite_flipbook, Empty, NewSprite_flipbook}
 * @param InOutPackagesUserRefusedToFullyLoad	A set of packages the user opted out of fully loading. This is used internally to prevent asking multiple times. { Empty }
 * @param InOutErrorMessage						A string with any errors that occurred during the rename.{ Empty }
 * @param ObjectToLanguageExtMap				A mapping of object to matching language (for fixing up localization if the objects are moved .  Note: Not used if bLocPackages is false { nullptr }
 * @param bLeaveRedirector						If true, a redirector will be left to allow unloaded assets to maintain a reference to the renamed object // false
 * @return true when the object was renamed successfully
 */
UNREALED_API bool RenameSingleObject(UObject* Object, FPackageGroupName& PGN, TSet<UPackage*>& InOutPackagesUserRefusedToFullyLoad, FText&InOutErrorMessage, const TMap< UObject*, FString >* ObjectToLanguageExtMap = NULL, bool bLeaveRedirector = true);
```
* 调用用例: D:\build\++UE4+Licensee\Sync\Engine\Source\Developer\AssetTools\Private\AssetRenameManager.cpp ---> PerformAssetRename()

```c++
/** Duplicates a list of objects
 *
 * @param	SelectedObjects			The objects to delete.选中的重命名的object数组
 * @param	SourcePath				A path to use to form a relative path for the copied objects. { Empty }
 * @param	DestinationPath			A path to use as a default destination for the copied objects. { /Game/Developers }
 * @param	OpenDialog				If true, a dialog will open to prompt the user for path information. { }
 * @param	OutNewObjects	If non-NULL, returns the list of duplicated objects. 可以是 nullptr
 */
 NREALED_API void DuplicateObjects( const TArray<UObject*>& SelectedObjects, const FString& SourcePath = TEXT(""), const FString& estinationPath = TEXT(""), bool bOpenDialog = true, TArray<UObject*>* OutNewObjects = NULL );

/** Duplicates a single object
 *
 * @param	Object									The objects to delete.
 * @param	PGN										The new package, group, and name of the object.
 * @param	InOutPackagesUserRefusedToFullyLoad		A set of packages the user opted out of fully loading. This is used internally to preven asking multiple times.\
 * @param	bPromptToOverwrite						If true the user will be prompted to overwrite if duplicating to an existing object.  I false, the duplication will always happen
 * @retun	The duplicated object or NULL if a failure occurred.
 */
 NREALED_API UObject* DuplicateSingleObject(UObject* Object, const FPackageGroupName& PGN, TSet<UPackage*>& nOutPackagesUserRefusedToFullyLoad, bool bPromptToOverwrite = true);
```
