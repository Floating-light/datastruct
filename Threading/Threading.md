# Process and Threading


## std::thread 
`std::thread`对象代表一个个执行线程。

`thread of execution`(执行线程)是一系列可以和其它指令序列在多线程环境中并发执行的指令序列，并共享同一地址空间。

一个被初始化的`thread`对象表示一个活跃的执行线程;这样的一个`thread`对象是`joinable`的，并且有一个唯一的`thread id`。

一个默认构造（未初始化）的`thread`对象不是`joinable`，它的`thread id`和所有`non-joinable`的线程一样。

当一个`joinable`是线程被`move`，或在其上调用`join`或`detach`，它会变为`not joinable`。

```c++
void join();
```

当线程执行完成时返回。

它同步了这个函数返回和线程中的所有操作都执行完的时刻。

