最终每个具体的Shader的声明
DECLARE_SHADER_TYPE(TBasePassPS,MeshMaterial);

```c++
// 声明新的Shader，应该在这个新的Shader 的class body中调用
#define DECLARE_SHADER_TYPE(ShaderClass,ShaderMetaTypeShortcut,...) \
	DECLARE_EXPORTED_SHADER_TYPE(ShaderClass,ShaderMetaTypeShortcut,, ##__VA_ARGS__)
```

```c++
#define DECLARE_EXPORTED_SHADER_TYPE(ShaderClass,ShaderMetaTypeShortcut,RequiredAPI, ...) \
	INTERNAL_DECLARE_SHADER_TYPE_COMMON(ShaderClass, ShaderMetaTypeShortcut, RequiredAPI); \
	DECLARE_EXPORTED_TYPE_LAYOUT(ShaderClass, RequiredAPI, NonVirtual); // Shader layout, 和前面一样
	public:
```

```c++
#define INTERNAL_DECLARE_SHADER_TYPE_COMMON(ShaderClass,ShaderMetaTypeShortcut,RequiredAPI) \
	public: \
	using ShaderMetaType = F##ShaderMetaTypeShortcut##ShaderType; \
	using ShaderMapType = F##ShaderMetaTypeShortcut##ShaderMap; \
	\
	static RequiredAPI ShaderMetaType StaticType; \
	\
	SHADER_DECLARE_VTABLE(ShaderClass)
```

```c++
#define SHADER_DECLARE_VTABLE(TBasePassPS) \
	static FShader* ConstructSerializedInstance() { return new TBasePassPS(); } \
	static FShader* ConstructCompiledInstance(const typename FShader::CompiledShaderInitializerType& Initializer) \
	{ return new TBasePassPS(static_cast<const typename ShaderMetaType::CompiledShaderInitializerType&>(Initializer)); }\
	static void ModifyCompilationEnvironmentImpl(const FShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) \
	{ \
		const typename TBasePassPS::FPermutationDomain PermutationVector(Parameters.PermutationId); \
		PermutationVector.ModifyCompilationEnvironment(OutEnvironment); \
		TBasePassPS::ModifyCompilationEnvironment(static_cast<const typename TBasePassPS::FPermutationParameters&>(Parameters), OutEnvironment); \
	} \
	static bool ShouldCompilePermutationImpl(const FShaderPermutationParameters& Parameters) \
	{ return TBasePassPS::ShouldCompilePermutation(static_cast<const typename TBasePassPS::FPermutationParameters&>(Parameters)); }
```

INTERNAL_DECLARE_SHADER_TYPE_COMMON最后展开为：
```c++
public: \
using ShaderMetaType = FMeshMaterialShaderType; \
using ShaderMapType = FMeshMaterialShaderMap; \
static RequiredAPI ShaderMetaType StaticType; 
static FShader* ConstructSerializedInstance() { return new TBasePassPS(); } \
static FShader* ConstructCompiledInstance(const typename FShader::CompiledShaderInitializerType& Initializer) \
{ return new TBasePassPS(static_cast<const typename ShaderMetaType::CompiledShaderInitializerType&>(Initializer)); }\
static void ModifyCompilationEnvironmentImpl(const FShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) \
{ \
	const typename TBasePassPS::FPermutationDomain PermutationVector(Parameters.PermutationId); \
	PermutationVector.ModifyCompilationEnvironment(OutEnvironment); \
	TBasePassPS::ModifyCompilationEnvironment(static_cast<const typename TBasePassPS::FPermutationParameters&>(Parameters), OutEnvironment); \
} \
static bool ShouldCompilePermutationImpl(const FShaderPermutationParameters& Parameters) \
{ return TBasePassPS::ShouldCompilePermutation(static_cast<const typename TBasePassPS::FPermutationParameters&>(Parameters)); }
```
其中FMeshMaterialShaderType是我们在外部自己定义好的ShaderType, 其作为静态成员，将初始化成唯一代表这个Shader的ShaderType, 并把自己注册到全局的链表中.