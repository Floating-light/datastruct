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
	DECLARE_EXPORTED_TYPE_LAYOUT(ShaderClass, RequiredAPI, NonVirtual); \
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
#define SHADER_DECLARE_VTABLE(ShaderClass) \
	static FShader* ConstructSerializedInstance() { return new ShaderClass(); } \
	static FShader* ConstructCompiledInstance(const typename FShader::CompiledShaderInitializerType& Initializer) \
	{ return new ShaderClass(static_cast<const typename ShaderMetaType::CompiledShaderInitializerType&>(Initializer)); }\
	static void ModifyCompilationEnvironmentImpl(const FShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) \
	{ \
		const typename ShaderClass::FPermutationDomain PermutationVector(Parameters.PermutationId); \
		PermutationVector.ModifyCompilationEnvironment(OutEnvironment); \
		ShaderClass::ModifyCompilationEnvironment(static_cast<const typename ShaderClass::FPermutationParameters&>(Parameters), OutEnvironment); \
	} \
	static bool ShouldCompilePermutationImpl(const FShaderPermutationParameters& Parameters) \
	{ return ShaderClass::ShouldCompilePermutation(static_cast<const typename ShaderClass::FPermutationParameters&>(Parameters)); }
```