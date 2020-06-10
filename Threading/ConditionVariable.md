# Condition Variable

考虑如下生产者与消费者的问题:
```c++
std::deque<int> q;
std::mutex mu;

// producer
void function_1()
{
    int count = 10;
    while(count > 0)
    {
        std::unique_lock<std::mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        std::this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}

// consumer
void function_2()
{
    int data = 0;
    while(data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        if(!q.empty())
        {
            data = q.back();
            q.pop_back();
            locker.unlock();
            cout << "t2 got a value from t1: " << data << endl;
        }
        else
        {
            locker.unlock();
        }
    }
}

int main()
{
    std::thread t1(function_1);
    std::thread t2(function_2);

    t1.join();
    t2.join();
    return 0;
}
```
`function_1`产生数据放到队列`q`中,`function_2`消费数据,即简单地打印出来。看起来没有问题，但分析`function_2`的执行过程,当队列中没有数据时,`if`中的条件判断失败,它在数据产生的间隔中总是处于无用的空循环中,这也叫`忙等待`。

一个直觉的想法是,在每次判断队列为空时`sleep`一段时间:
```c++
std::this_thread::sleep_for(chrono::seconds(10));
```
但是这个时间的长短又难以把握, 长了，无法即使处理数据,短了, 效果又不明显。

另外一种方式是使用条件变量:
```c++
std::condition_variable cond;
```
在`t2`中产生数据后, 调用`cond.notify_one()`通知一个在等待这个条件`cond`的线程.

在`t1`中我们调用`cond.wait(locker)`来等待`cond`条件被通知发生，在`wait()`的参数中传入`locker`，会首先`unlock()`它,然后进入等待,等到被通知之后再`lock()`它. 
```c++
cond.wait(locker);
```
通常只想`t1`线程在队列中有数据的时候被唤醒, 但也可能被自己唤醒, 称为spurious wake, 解决这一问题可以在`wait()`的第二个参数传入一个一元谓词，这里我们加一个lambda限制唤醒条件为仅在队列`q`不为空时。
```c++
cond.wait(locker, [](){return !q.empty();});
```
全部代码为:
```c++
std::deque<int> q;
std::mutex mu;
std::condition_variable cond;
// producer
void function_1()
{
    int count = 10;
    while(count > 0)
    {
        std::unique_lock<std::mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        // cond.notify_all(); 
        cond.notify_one(); // Notify one waiting thread, if there is one.
        std::this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}

// consumer
void function_2()
{
    int data = 0;
    while(data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        //cond.wait(locker); // first unlock and go to sleep, lock it again after wake up .                
        cond.wait(locker, [](){return !q.empty();}); // 通常只想它在队列中有数据的时候被唤醒, 但也可能被自己唤醒, 称为spurious wake.
                                                     // 因此加一个lambda限制唤醒条件.
        data = q.back();
        q.pop_back();
        locker.unlock();
        cout << "t2 got a value from t1: " << data << endl;
    }
}

int main()
{
    std::thread t1(function_1);
    std::thread t2(function_2);

    t1.join();
    t2.join();
    return 0;
}
```
