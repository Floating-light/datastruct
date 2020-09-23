在UE的编辑器中,我们可以通过一些简单的操作在ContentBrowser中创建并编辑一些对象(即资源).
* 在ContentBrowser空白处单击右键创建资源.
* 在ContentBrowser选中一个或多个资源,右键从中创建新的资源.
* 自定义特定资源资源编辑器(双击资源时打开的窗口)

参照`Paper2D`的实现方式.

1. 实现资产的C++类, 继承自UObject.

