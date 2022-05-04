声明顶点工厂
```c++
class ENGINE_API FLocalVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FLocalVertexFactory);
public:
    ... ...
};
#define DECLARE_VERTEX_FACTORY_TYPE(FactoryClass) \
	public: \
	static FVertexFactoryType StaticType; \
	virtual FVertexFactoryType* GetType() const override;
```

定义实现
```c++
IMPLEMENT_VERTEX_FACTORY_TYPE_EX(FLocalVertexFactory,"/Engine/Private/LocalVertexFactory.ush",true,true,true,true,true,true,true);
```

```c++
#define IMPLEMENT_VERTEX_FACTORY_TYPE_EX(FactoryClass,ShaderFilename,bUsedWithMaterials,bSupportsStaticLighting,bSupportsDynamicLighting,bPrecisePrevWorldPos,bSupportsPositionOnly,bSupportsCachingMeshDrawCommands,bSupportsPrimitiveIdStream) \
	IMPLEMENT_TEMPLATE_VERTEX_FACTORY_TYPE_EX(,FactoryClass,ShaderFilename,bUsedWithMaterials,bSupportsStaticLighting,bSupportsDynamicLighting,bPrecisePrevWorldPos,bSupportsPositionOnly,bSupportsCachingMeshDrawCommands,bSupportsPrimitiveIdStream)
```

```c++
#define IMPLEMENT_TEMPLATE_VERTEX_FACTORY_TYPE_EX(TemplatePrefix, FactoryClass,ShaderFilename,bUsedWithMaterials,bSupportsStaticLighting,bSupportsDynamicLighting,bPrecisePrevWorldPos,bSupportsPositionOnly,bSupportsCachingMeshDrawCommands,bSupportsPrimitiveIdStream) \
	PREPROCESSOR_REMOVE_OPTIONAL_PARENS(TemplatePrefix) FVertexFactoryType FactoryClass::StaticType( \
		TEXT(#FactoryClass), \
		TEXT(ShaderFilename), \
		bUsedWithMaterials, \
		bSupportsStaticLighting, \
		bSupportsDynamicLighting, \
		bPrecisePrevWorldPos, \
		bSupportsPositionOnly, \
		bSupportsCachingMeshDrawCommands, \
		bSupportsPrimitiveIdStream, \
		IMPLEMENT_VERTEX_FACTORY_VTABLE(FactoryClass) \
		); \
		PREPROCESSOR_REMOVE_OPTIONAL_PARENS(TemplatePrefix) FVertexFactoryType* FactoryClass::GetType() const { return &StaticType; }
```