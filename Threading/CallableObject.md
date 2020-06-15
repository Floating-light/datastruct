# 可调用对象
C++中有多种可调用对象,他们可以作为参数传给`std::bind()`,`std::thread()`, `std::async()`,`std::call_once()`等。

考虑我们有这样一个类:
```c++
class A
{
public:
void f(int x, char c){}
long g(double x) { return 0;}
int operator()(int N){return 0;}
};
```
## operator()
对于重载了`()`操作符的类的实例可以直接当作可调用对象调用，且可以copy,ref,move.
```c++
std::thread(a, 6); // copy of a() in a different thread
std::thread(std::ref(a), 6); // a() in a different thread
std::thread(std::move(a), 6); // a is no longer ysable in main thread
```
## Lambda
```c++
std::thread([](int x){return x*x;}, 6);
```
## 普通函数指针
```c++
void foo(int x) { }
int main()
{
    std::thread(foo, 6);
}
```
### 成员函数
```c++
std::thread(&A::f,a, 6, 'c'); // copy of a .f(8,'c') in a different thread
std::thread(&A::f,std::ref(a), 6, 'c'); // a.f(8,'c') in a different thread
```

## packaged_task
类似于`funciton`, `packaged_task`可以绑定一个可调用对象, 并执行,但是它的返回类型是`void`，获取它的返回值必须用`functrue`:
```c++
int main()
{
    // 如何给它传入固定参数, 而不必在调用时指定
    std::packaged_task<int(int)> t(factorial);
    std::packaged_task<int()> t(std::bind(factorial, 6));

    // do something else

    t(); // in a different context， always return void
    int x = t.get_future().get();
    std::cout << x << std::endl;
    return 0;
}
```
由此可以看出,它和`fuction`的不同之处在于`packaged_task`把一个可调用对象链接到了未来，用于多线程执行。

考虑这样一个场景,多个线程共享一个任务队列,一个线程负责产生任务,并将任务放到任务队列中, 还要在这个任务执行后获取它的返回值.多个子线程从任务队列中取出任务并执行.这里简化一下这个场景,主线程产生任务，一个子线程`t1`执行。
```c++
std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;
void thread_1()
{
    std::packaged_task<int()> t;
    {
        // std::lock_guard<std::mutex> locker(mu);
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, [](){ return !task_q.empty();});
        t = std::move(task_q.front());
        task_q.pop_front();
    }
    t();
}

int main()
{
    std::thread t1(thread_1);

    std::packaged_task<int()> t(std::bind(factorial, 6));
    std::future<int> res = t.get_future();
    {
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(std::move(t));
    }
    cond.notify_one();

    t1.join();
    std::cout << "result int main thread: " << res.get() << std::endl;
    return 0;
}
```

## c++ 线程总结
```c++
int main()
{
    /* thread */
    std::thread t1(factorial, 6);
    std::this_thread::sleep_for(chrono::milliseconds(3));
    std::chrono::steady_clock::time_point tp = chrono::steady_clock::now() + chrono::microseconds(4);
    std::this_thread::sleep_until(tp);

    /* Mutex */
    std::mutex mu;
    std::lock_guard<std::mutex> locker(mu);
    std::unique_lock<std::mutex> locker2(mu);
    locker2.try_lock();
    locker2.try_lock_for(chrono::nanoseconds(500));
    locker2.try_lock_until(tp);

    /* Condition variable*/
    std::condition_variable cond;
    cond.wait_for(locker2, chrono::microseconds(2));
    cond.wait_until(locker2, tp);
    
    /* Future and Promise */
    std::promise<int> p;
    std::future<int> f = p.get_future();    
    f.get();
    f.wait();
    f.wait_for(std::chrono::microseconds(2));
    f.wait_until(tp);

    /* async() */
    std::future<int> fu = async(factorial, 6);

    /* Packaged Task*/
    std::packaged_task<int(int)> t(factorial);
    std::future<int> fu2 = t.get_future();
    t(6);
}
```