// ProcessDefaultParam
struct Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam_Statics::Function_MetaDataParams[] = {
		{ "Comment", "//BlueprintImplementableEvents in Interfaces must not be declared 'virtual'\n// ??\xcd\xbc\xca\xb5??, c++ ????Excute_ProcessDefaultParam()????\n" },
		{ "ModuleRelativePath", "Public/Frame/Base/Interface/PawnControllerInterface.h" },
		{ "ToolTip", "BlueprintImplementableEvents in Interfaces must not be declared 'virtual'\n ??\xcd\xbc\xca\xb5??, c++ ????Excute_ProcessDefaultParam()????" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam_Statics::FuncParams = { 
        (UObject*(*)())Z_Construct_UClass_UPawnControllerInterface,
        nullptr, 
        "ProcessDefaultParam", 
        nullptr, 
        nullptr, 
        0, 
        nullptr, 
        0, 
        RF_Public|RF_Transient|RF_MarkAsNative, 
        (EFunctionFlags)0x08020800, 
        0, 
        0, 
        METADATA_PARAMS(Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam_Statics::Function_MetaDataParams, 
        UE_ARRAY_COUNT(Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam_Statics::Function_MetaDataParams)) };

	UFunction* Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPawnControllerInterface_ProcessDefaultParam_Statics::FuncParams);
		}
		return ReturnFunction;
	}




    // TestBPNative
	struct Z_Construct_UFunction_UPawnControllerInterface_TestBPNative_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPawnControllerInterface_TestBPNative_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Frame/Base/Interface/PawnControllerInterface.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPawnControllerInterface_TestBPNative_Statics::FuncParams = { 
        (UObject*(*)())Z_Construct_UClass_UPawnControllerInterface, 
        nullptr, 
        "TestBPNative", 
        nullptr, 
        nullptr, 
        0, 
        nullptr, 
        0, 
        RF_Public|RF_Transient|RF_MarkAsNative, 
        (EFunctionFlags)0x08020C00,
        0, 
        0, 
        METADATA_PARAMS(Z_Construct_UFunction_UPawnControllerInterface_TestBPNative_Statics::Function_MetaDataParams, 
        UE_ARRAY_COUNT(Z_Construct_UFunction_UPawnControllerInterface_TestBPNative_Statics::Function_MetaDataParams)) };
	
    UFunction* Z_Construct_UFunction_UPawnControllerInterface_TestBPNative()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPawnControllerInterface_TestBPNative_Statics::FuncParams);
		}
		return ReturnFunction;
	}