* 文档: https://docs.unrealengine.com/zh-CN/ProgrammingAndScripting/Slate/DetailsCustomization/index.html
* struct details custom: https://zhuanlan.zhihu.com/p/77051015
* 引擎用例: UE_4.26\Engine\Source\Editor\DetailCustomizations\Public\DetailCustomizations.h

分为属性和对象两类自定义, 属性Detail 自定义改变所有这个属性在Detail 面板中出现的方式. 对象 detail 自定义改变这个对象的Details 面板.

结构体需要用一个派生自IPropertyTypeCustomization的类实现, 并在模块StartupModule中注册.对象类型的自定义也是如此, 只是需要派生自IDetailCustomization