每一个DECLARE_SHADER_TYPE，在对应的cpp文件都有一个IMPLEMENT_MATERIAL_SHADER_TYPE.
```c++
// 针对材质
#define IMPLEMENT_MATERIAL_SHADER_TYPE(TemplatePrefix,ShaderClass,SourceFilename,FunctionName,Frequency) 
	IMPLEMENT_SHADER_TYPE( 
		TemplatePrefix, 
		ShaderClass, 
		SourceFilename, 
		FunctionName, 
		Frequency 
		);
```
```c++
// 实例化一个Shader模板类，随后实现这个ShaderType
typedef TBasePassPS<LightMapPolicyType, bEnableSkyLight> TBasePassPS##LightMapPolicyName##SkyLightName; \
	IMPLEMENT_MATERIAL_SHADER_TYPE(template<>,TBasePassPS##LightMapPolicyName##SkyLightName,TEXT("/Engine/Private/BasePassPixelShader.usf"),TEXT("MainPS"),SF_Pixel);
```

```c++
#define IMPLEMENT_SHADER_TYPE(TemplatePrefix,ShaderClass,SourceFilename,FunctionName,Frequency) \
	IMPLEMENT_UNREGISTERED_TEMPLATE_TYPE_LAYOUT(TemplatePrefix, ShaderClass); \
	TemplatePrefix \
	ShaderClass::ShaderMetaType ShaderClass::StaticType( \
		ShaderClass::StaticGetTypeLayout(), \
		TEXT(#ShaderClass), \
		SourceFilename, \
		FunctionName, \
		Frequency, \
		ShaderClass::FPermutationDomain::PermutationCount, \
		SHADER_TYPE_VTABLE(ShaderClass), \
		sizeof(ShaderClass), \
		ShaderClass::GetRootParametersMetadata() \
		);
```

全部展开后：
```c++
#define IMPLEMENT_SHADER_TYPE(TemplatePrefix,ShaderClass,SourceFilename,FunctionName,Frequency) \
	// IMPLEMENT_UNREGISTERED_TEMPLATE_TYPE_LAYOUT(TemplatePrefix, ShaderClass);  // 实现自己的TypeLayout
    	(TemplatePrefix) void (T)::InternalDestroy(void* Object, const FTypeLayoutDesc&, const FPointerTableBase* PtrTable) { \
		Freeze::CleanupObject(static_cast<(T)*>(Object), PtrTable); \
		Freeze::CallDestructor(static_cast<(T)*>(Object)); \
	} \
	TemplatePrefix FTypeLayoutDesc& (T)::StaticGetTypeLayout() { \
		static_assert(TValidateInterfaceHelper<(T), InterfaceType>::Value, "Invalid interface for " #T); \
		alignas(FTypeLayoutDesc) static uint8 TypeBuffer[sizeof(FTypeLayoutDesc)] = { 0 }; \
		FTypeLayoutDesc& TypeDesc = *(FTypeLayoutDesc*)TypeBuffer; \
		if (!TypeDesc.IsInitialized) { \
			TypeDesc.IsInitialized = true; \
			TypeDesc.Name = TEXT(#T); \
			TypeDesc.WriteFrozenMemoryImageFunc = TGetFreezeImageHelper<(T)>::Do(); \
			TypeDesc.UnfrozenCopyFunc = &Freeze::DefaultUnfrozenCopy; \
			TypeDesc.AppendHashFunc = &Freeze::DefaultAppendHash; \
			TypeDesc.GetTargetAlignmentFunc = &Freeze::DefaultGetTargetAlignment; \
			TypeDesc.ToStringFunc = &Freeze::DefaultToString; \
			TypeDesc.DestroyFunc = &InternalDestroy; \
			TypeDesc.Size = sizeof((T)); \
			TypeDesc.Alignment = alignof((T)); \
			TypeDesc.Interface = InterfaceType; \
			TypeDesc.SizeFromFields = ~0u; \
			TypeDesc.GetDefaultObjectFunc = &TGetDefaultObjectHelper<(T), InterfaceType>::Do; \
			InternalLinkType<1>::Initialize(TypeDesc); \
			InternalInitializeBases<(T)>(TypeDesc); \
			FTypeLayoutDesc::Initialize(TypeDesc); \
		} \
		return TypeDesc; }
    const FTypeLayoutDesc& (T)::GetTypeLayout() const { return StaticGetTypeLayout(); }
	TemplatePrefix 
	ShaderClass::ShaderMetaType ShaderClass::StaticType( // 静态初始化自己的ShaderType，sahder源文件也在这里处理
		ShaderClass::StaticGetTypeLayout(), 
		TEXT(#ShaderClass), 
		SourceFilename, 
		FunctionName, 
		Frequency, 
		ShaderClass::FPermutationDomain::PermutationCount, 
		SHADER_TYPE_VTABLE(ShaderClass), 
		sizeof(ShaderClass), 
		ShaderClass::GetRootParametersMetadata() 
		);
```

所以实际上DECLARE_SHADER_TYPE，IMPLEMENT_MATERIAL_SHADER_TYPE这对宏包括了DECLARE_TYPE_LAYOUT和IMPLEMENT_TYPE_LAYOUT，只是加上了定义ShaderType，并自动注册的代码。
这里根据模板参数的不同，定义许多BasePassPS的变体，LightMap, SkyLight等。下面有一个函数GetUniformBasePassShaders通过对应的模板参数Get到这些Shader.直接通过获取StaticType.