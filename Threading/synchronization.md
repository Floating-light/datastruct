# 同步(synchronization)

协作进程(cooperating)

进程同步(process synchronization)

进程协调(process synchronization)

### Peterson 解决方案
两进程共享变量:
```c++
int turn; // 哪一个进程可进入临界区
bool flag[2]; // 哪一个进程准备进入临界区

// 进程Pi
do {
    flag[i] = true;
    turn = j;
    while(flag[j] && turn  == j);
    // 临界区
    flag[i] = false;
    //剩余区
}while(true);
```

基于软件的解决方案(Peterson解决方案)并不保证在现代计算机体系结构上正确工作。

    内核开发人员————硬件
    应用程序员————软件API

