// 当我们需要将一些对象搜集起来集中管理时, 通常需要将这些对象放到一个容器中.

#define DOREPLIFETIME_CONDITION_NOTIFY(c,v,cond,rncond) \
{ \
	FDoRepLifetimeParams LocalDoRepParams; \
	LocalDoRepParams.Condition = cond; \
	LocalDoRepParams.RepNotifyCondition = rncond; \
	DOREPLIFETIME_WITH_PARAMS(c,v,LocalDoRepParams); \
}

#define DOREPLIFETIME_WITH_PARAMS(c,v,params) \
{ \
	FProperty* ReplicatedProperty = GetReplicatedProperty(StaticClass(), c::StaticClass(),GET_MEMBER_NAME_CHECKED(c,v)); \
	RegisterReplicatedLifetimeProperty(ReplicatedProperty, OutLifetimeProps, params); \
}


FDoRepLifetimeParams LocalDoRepParams; 
LocalDoRepParams.Condition = COND_None; 
LocalDoRepParams.RepNotifyCondition = REPNOTIFY_Always; 
FProperty* ReplicatedProperty = GetReplicatedProperty(StaticClass(), UGDAttributeSetBase::StaticClass(),GET_MEMBER_NAME_CHECKED(UGDAttributeSetBase,Health)); \
RegisterReplicatedLifetimeProperty(ReplicatedProperty, OutLifetimeProps, LocalDoRepParams);
