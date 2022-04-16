```c++
LAYOUT_FIELD(int32, ResourceIndex);

#define LAYOUT_FIELD(T, Name, ...) 
PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T) Name; // 类型中可能存在逗号，则需要加括号，这里把括号去掉
INTERNAL_LAYOUT_FIELD(T, Name, STRUCT_OFFSET(DerivedType, Name), EFieldLayoutFlags::MakeFlags(__VA_ARGS__), 1u, 0u, __COUNTER__)

STRUCT_OFFSET(DerivedType, Name) // 返回一个数字，当前属性在结构体中的地址偏移，DerivedType当前类型
__COUNTER__ 编译器支持的预处理宏，编译一次+1

INTERNAL_LAYOUT_FIELD(int32, ResourceIndex, 1575, EFieldLayoutFlags::MakeFlags(__VA_ARGS__), 1u, 0u, 0)
UE_DECLARE_INTERNAL_LINK_SPECIALIZATION(InternalLinkType, Counter - CounterBase) { \
		UE_STATIC_ONLY(InternalLinkType); \
		static void Initialize(FTypeLayoutDesc& TypeDesc) { \
			InternalLinkType<Counter - CounterBase + 1>::Initialize(TypeDesc); \
			alignas(FFieldLayoutDesc) static uint8 FieldBuffer[sizeof(FFieldLayoutDesc)] = { 0 }; \
			FFieldLayoutDesc& FieldDesc = *(FFieldLayoutDesc*)FieldBuffer; \
			FieldDesc.Name = TEXT(#InName); \
			FieldDesc.UFieldNameLength = Freeze::FindFieldNameLength(FieldDesc.Name); \
			FieldDesc.Type = &StaticGetTypeLayoutDesc<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>(); \
			FieldDesc.WriteFrozenMemoryImageFunc = TGetFreezeImageFieldHelper<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>::Do(); \
			FieldDesc.Offset = InOffset; \
			FieldDesc.NumArray = InNumArray; \
			FieldDesc.Flags = InFlags; \
			FieldDesc.BitFieldSize = InBitFieldSize; \
			FieldDesc.Next = TypeDesc.Fields; \
			TypeDesc.Fields = &FieldDesc; \
		} \
	}; \

// CounterBase 在定义DECLARE_TYPE_LAYOUT时生成
// Counter - CounterBase = 1 --> 第几个属性
template<> struct InternalLinkType<1>
{
    static void Initialize(FTypeLayoutDesc& TypeDesc) { 
        // 先初始化下一个
    	InternalLinkType<Counter - CounterBase + 1>::Initialize(TypeDesc); 
    	alignas(FFieldLayoutDesc) static uint8 FieldBuffer[sizeof(FFieldLayoutDesc)] = { 0 }; 
        // 相当于定义了一个静态FFieldLayoutDesc，保存当前字段的反射信息
    	FFieldLayoutDesc& FieldDesc = *(FFieldLayoutDesc*)FieldBuffer; 
    	FieldDesc.Name = TEXT(#InName); 
    	FieldDesc.UFieldNameLength = Freeze::FindFieldNameLength(FieldDesc.Name); 
        // FTypeLayoutDesc*,获取类型Desc，里面定义了内存布局的基本信息和构造和析构相关的Func.每个支持这种反射的类型都要定义
        // DECLARE_INTRINSIC_TYPE_LAYOUT(int)
    	FieldDesc.Type = &StaticGetTypeLayoutDesc<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>(); 
    	FieldDesc.WriteFrozenMemoryImageFunc = TGetFreezeImageFieldHelper<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>::Do(); 
    	FieldDesc.Offset = InOffset; 
    	FieldDesc.NumArray = InNumArray; 
    	FieldDesc.Flags = InFlags; 
    	FieldDesc.BitFieldSize = InBitFieldSize; 
    	FieldDesc.Next = TypeDesc.Fields; // TypeDesc.Fields 是下一个fields的Desc
    	TypeDesc.Fields = &FieldDesc;  // 当前Fields
    }
}
```

```c++
DECLARE_TYPE_LAYOUT(FShader, NonVirtual);
#define DECLARE_TYPE_LAYOUT(T, Interface) INTERNAL_DECLARE_LAYOUT_BASE(T); INTERNAL_DECLARE_TYPE_LAYOUT(T, Interface, PREPROCESSOR_NOTHING)

private: using InternalBaseType = typename TGetBaseTypeHelper<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>::Type; 
template<typename InternalType> static void InternalInitializeBases(FTypeLayoutDesc& TypeDesc) 
{ TInitializeBaseHelper<InternalType, InternalBaseType>::Initialize(TypeDesc); }

private: static void InternalDestroy(void* Object, const FTypeLayoutDesc&, const FPointerTableBase* PtrTable); 
public: RequiredAPI static FTypeLayoutDesc& StaticGetTypeLayout(); 
public: (virtual ) const FTypeLayoutDesc& GetTypeLayout() const ;
// INTERNAL_DECLARE_TYPE_LAYOUT_COMMON(T, InInterface)

static const int CounterBase = __COUNTER__; 
public: using DerivedType = PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T); 
static const ETypeLayoutInterface::Type InterfaceType = ETypeLayoutInterface::InInterface; 
// UE_DECLARE_INTERNAL_LINK_BASE(InternalLinkType)
template<int Counter> struct InternalLinkType { 
	UE_STATIC_ONLY(InternalLinkType); // Empty
 	static FORCEINLINE void Initialize(FTypeLayoutDesc&TypeDesc) {} 
}
```

```c++
IMPLEMENT_TYPE_LAYOUT(FShader);

IMPLEMENT_TEMPLATE_TYPE_LAYOUT(, T)

// Expand 
1. IMPLEMENT_UNREGISTERED_TEMPLATE_TYPE_LAYOUT(TemplatePrefix, T); \
2. INTERNAL_REGISTER_TYPE_LAYOUT(T)

1-> Expand (,FShader)
INTERNAL_IMPLEMENT_TYPE_LAYOUT_COMMON(, FShader); 
	PREPROCESSOR_REMOVE_OPTIONAL_PARENS(TemplatePrefix) void PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)::InternalDestroy(void* Object, const FTypeLayoutDesc&, const FPointerTableBase* PtrTable) { 
		Freeze::CleanupObject(static_cast<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)*>(Object), PtrTable); 
		Freeze::CallDestructor(static_cast<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)*>(Object)); 
	} 
	FTypeLayoutDesc& T::StaticGetTypeLayout() { 
		static_assert(TValidateInterfaceHelper<T, InterfaceType>::Value, "Invalid interface for " #T); 
		alignas(FTypeLayoutDesc) static uint8 TypeBuffer[sizeof(FTypeLayoutDesc)] = { 0 }; 
		FTypeLayoutDesc& TypeDesc = *(FTypeLayoutDesc*)TypeBuffer; 
		if (!TypeDesc.IsInitialized) { 
			TypeDesc.IsInitialized = true; 
			TypeDesc.Name = TEXT(#T); 
			TypeDesc.WriteFrozenMemoryImageFunc = TGetFreezeImageHelper<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>::Do(); 
			TypeDesc.UnfrozenCopyFunc = &Freeze::DefaultUnfrozenCopy; 
			TypeDesc.AppendHashFunc = &Freeze::DefaultAppendHash; 
			TypeDesc.GetTargetAlignmentFunc = &Freeze::DefaultGetTargetAlignment; 
			TypeDesc.ToStringFunc = &Freeze::DefaultToString; 
			TypeDesc.DestroyFunc = &InternalDestroy; 
			TypeDesc.Size = sizeof(PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)); 
			TypeDesc.Alignment = alignof(PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)); 
			TypeDesc.Interface = InterfaceType; 
			TypeDesc.SizeFromFields = ~0u; 
			TypeDesc.GetDefaultObjectFunc = &TGetDefaultObjectHelper<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T), InterfaceType>::Do; 
			InternalLinkType<1>::Initialize(TypeDesc); 
			InternalInitializeBases<PREPROCESSOR_REMOVE_OPTIONAL_PARENS(T)>(TypeDesc); 
			FTypeLayoutDesc::Initialize(TypeDesc); 
		} 
		return TypeDesc; }
const FTypeLayoutDesc& FShader::GetTypeLayout() const { return StaticGetTypeLayout(); }

2-> Expand T-->FShader
static const FDelayedAutoRegisterHelper PREPROCESSOR_JOIN(DelayedAutoRegisterHelper, __COUNTER__)(DelayedAutoRegisterHelper)(EDelayedRegisterRunPhase::ShaderTypesReady, []{ FTypeLayoutDesc::Register(T::StaticGetTypeLayout()); });
```
