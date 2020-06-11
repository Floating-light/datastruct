当一个线程需要它的一个子线程执行的函数的返回值时改如何处理?

考虑如下函数:
```c++
void factorial(int N, int& x)
{
    int res = 1;
    for(int i = N; i > 1; i--)
    {
        res *= i;
    }
    cout << "Result is : " << res << endl;
    x = res;
}
```
它计算N的阶乘,并将结果给x返回.如果用一个子线程计算,则可以像这样写:
```c++
int main()
{
    int x;
    std::thread t1(factorial, 4, std::ref(x));

    t1.join();
    return 0;
}
```
由于`x`是两个线程共享的变量,我们需要`std:mutex`来同步两个线程对`x`的访问.此外,我们还希望主线程在子线程计算好`x`之后才能使用`x`,所以我们还需要一个`std::condition_variable`，由子线程通知在等待的线程`x`已经计算好了.一切又变得如此复杂!

我们可以使用`std::future`, 它可以表示一个用于获取未来的值的对象,同时使用`std::async`来执行`factorial`.
先将`factorial`的结果改成返回值(而不是引用参数)。
```c++
int main()
{
    int x;
    std::future<int> fu = std::async(factorial, 4);

    x = fu.get();// 等到子线程完成, 并获取返回值。只能调用一次
    //int y = fu.get(); // crash
    return 0;
}
```

此外,`std::async`的第一个参数用来表明是否立即创建线程执行传如的函数:
```c++
// std::launch::deferred 使用这一参数将不会创建新的线程，也就不会立即执行factorial
// 直到你调用get, factorial就会立即在同一线程中执行。
std::future<int> fu = std::async(std::launch::deferred,factorial, 4);

// 会创建另一个线程.并立即执行
std::future<int> fu = std::async(std::launch::async,factorial, 4);

// 和std::launch::async一样
std::future<int> fu = std::async(std::launch::async | std::launch::deferred,factorial, 4);
```

### 从父线程向子线程传参数
由于一些原因,在调用`std::async`时,我们可能还不知道要传递的参数的值,要等到未来的某一时刻才知道.

先将`factorial`传入的参数`N`改为`std::future<int>&`从中`get()`得到`N`的值。

```c++
int main()
{
    // 想传给子线程一个int参数,但调用的时候不知道这个参数的具体值,
    // 所以我们承诺会在未来给它一个值.
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));

    // do something else
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    p.set_value(4);

    int x = fu.get();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Get from child: " << x << std::endl;

    return 0;
}
```
将我们需要传入的值用`std::promise<int>`表示,意为我们承诺会在未来某个时刻设定这个值，并从中调用`get_future()`获取一个`std::future<int>`对象,表示这个未来的值,并作为参数传入。在后面,我们可以调用`p.set_value(4)`兑现我们承诺, 之后,子线程的`f.get()`得以返回,并进一步执行。

如果后面没有兑现承诺`set_value`,则会在这个`std::promise<int> p`返回的`future`对象的`get()`函数中出现异常:
```c++
exception: std::future_errc::broken_promise
```

如果后面实在无法兑现承诺,可以主动设置一个异常:
```c++
p.set_exception(std::make_exception_ptr(std::runtime_error("To err is human")));
```

如果有多个线程需要处理同一个参数,而且这个参数还是`promise`的，我们可以用`shared_future`，它是可以复制的,所以需要将`factorial`的参数改为`std::shared_future<int>`。
```c++
int main()
{
    int x = 0;
    // 想传给子线程一个int参数,但调用的时候不知道这个参数的具体值,
    // 所以我们承诺会在未来给它一个值.
    std::promise<int> p;
    std::future<int> f = p.get_future();

    // can be copy 
    std::shared_future<int> sf = f.share();

    std::future<int> fu1 = std::async(std::launch::async, factorial, sf);
    std::future<int> fu2 = std::async(std::launch::async, factorial, sf);
    std::future<int> fu3 = std::async(std::launch::async, factorial, sf);
    std::future<int> fu4 = std::async(std::launch::async, factorial, sf);
    // ... many many

    // do something else
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    p.set_value(4);

    int x = fu1.get();
    std::cout << "Get from child: " << x << std::endl;
    std::cout << "Get from child: " << fu2.get() << std::endl;
    std::cout << "Get from child: " << fu3.get() << std::endl;
    std::cout << "Get from child: " << fu4.get() << std::endl;
    return 0;
}
```
这样,在调用了`p.set_value(4)`之后，四个线程都会收到同一个值。

这在采用多播通信模型时很有用。

reference:
1. http://www.cplusplus.com/reference/future/
2. https://www.youtube.com/watch?v=SZQ6-pf-5Us&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M&index=7