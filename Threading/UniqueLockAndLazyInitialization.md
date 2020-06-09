# Unique Lock

前面提到了两种加锁的办法, 这里还有第三种。
1. std::lock_guard
2. std::mutex 的lock()和unlock()方法. 
3. std::unique_lock
第一种方法是推荐的加锁方式,极不推荐第二种方式。第三种`unique_lock`提供了更加灵活的加锁,释放锁的方式.

`lock_guard`仅仅实现了在构造函数中释放锁和初始化加锁(也可以指定`std::adopt_lock`参数不加), 其拷贝构造和赋值操作符均为`delete`:
```c++
lock_guard(const lock_guard&) = delete;
lock_guard& operator=(const lock_guard&) = delete;
```

`unique_lock`在此基础上实现了更加多样且灵活的操作,提供了更加细粒度的同步操作，由此带来的自然就有性能方面的损耗.

而`unique_lock`首先可以像`lock_guard`那样使用
```c++
void shared_print(string id, int value)
{
    // construct and lock
    std::unique_lock<std::mutex> locker(_mu);
    cout << "From " << id << ": " << value << std::endl;
}
```
也可以先不加锁,随后执行一些无需加锁的操作,直到真正需要同步的地方再加锁,用完就立马释放,无需等到程序执行出当前范围，从而支持了更加细粒度的加锁。
```c++
void shared_print(string id, int value)
{
    // construct but don't lock
    std::unique_lock<std::mutex> locker(_mu, std::defer_lock);

    // ... do something else does not access the ofstream

    locker.lock();
    cout << "From " << id << ": " << value << std::endl;
    locker.unlock();

    // ...do something else.
}
```
也可以多次加锁、释放锁, 这在有复杂逻辑时可以实现最小粒度的加锁。此外`unique_lock`有移动构造。
```c++
void shared_print(string id, int value)
{
    // construct but don't lock
    std::unique_lock<std::mutex> locker(_mu, std::defer_lock);

    // ... do something else does not access the ofstream

    locker.lock();
    cout << "From " << id << ": " << value << std::endl;
    locker.unlock();

    // ...do something else.

    locker.lock(); // 可以lock 和unlock 任意次数

    // 可以move
    std::unique_lock<std::mutex> locker2= std::move(locker);
    
    // release in the deconstructor of locker2
}
```
# Lazy Initialization
考虑在在构造函数中打开`log`文件的行为还是有点不妥，如果程序从未调用`shared_print`,就没必要打开它。因此,我们需要在`shared_print`中打开它,所以需要确保线程安全,就需要加锁,由于这个锁只是用来确保文件正确打开的,不会像`_mu`那样被频繁用于同步`_f`的访问，所以一般要用一个新的锁:

```c++
void shared_print(string id, int value)
{
    // Lazy Initialization
    // or initialization upon first use Idiom
    if(!_f.is_open())
    {
        std::unique_lock<std::mutex> locker2(_mu_open); // still not thread safe
        _f.open("log.txt"); // make sure thread safe， only call once.
    }

    std::lock_guard<std::mutex> locker(_mu);
    cout << "From " << id << ": " << value << std::endl;
}
```
但此时仍不是线程安全的,因为多个线程可能同时通过`if`语句的判断进入获得锁的等待中(一个获得锁其它等待), 当第一个获得锁的线程打开文件并释放锁后,之前在等待锁的线程仍然会尝试打开文件,所以还是会出现多次打开.我们只能将判断文件是否打开的语句也包含在锁的保护之下:
```c++
void shared_print(string id, int value)
{
    // Lazy Initialization
    // or initialization upon first use Idiom
    {
        // that's good
        std::unique_lock<std::mutex> locker2(_mu_open);
        if(!_f.is_open())
        {
            // std::unique_lock<std::mutex> locker2(_mu_open); // still not thread safe
            _f.open("log.txt"); // thread safe
        }
    }
    std::call_once(_flag, [&](){ _f.open("log.txt");}); // file will be opened only once by one thread
    
    std::lock_guard<std::mutex> locker(_mu);
    cout << "From " << id << ": " << value << std::endl;
}
```
但是,这样一来,每次调用`shared_print`都需要经历一次`_mu_open`锁的获得和释放,这极大地影响了并行性能,我们仅仅需要用一次,却为此付出了太多的代价.

这类问题的一个解决办法是标准库提供的`call_once`函数。

```c++
// ...
//member 
std::once_flag _flag;
// ... 
void shared_print(string id, int value)
{
    std::call_once(_flag, [&](){ _f.open("log.txt");}); // file will be opened only once by one thread
    
    std::lock_guard<std::mutex> locker(_mu);
    cout << "From " << id << ": " << value << std::endl;
}
```
定义一个`std::once_flag`成员变量，将其作为参数传给`std::call_once`，它的第二个参数是一个可调用对象,且只会执行一次。将一个调用`_f.open()`函数的`Lambda`传给它就实现了上述`Lazy Initialization`的功能。
