// Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdlib.h>
#include <Windows.h>

#include <iostream>
#include <thread>
#include <hash_map>


#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

void testFunc1()
{
    std::cout << "testFunc1 ^-^" << std::endl;
}

void process()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(pi);
    ZeroMemory(&pi, sizeof(pi));
    wchar_t wc[255] = TEXT("C:\\Users\\MY\\Desktop\\DemoV4\\GenTexture.exe");
    if (!CreateProcess(NULL, // use command line
        wc, // command
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        fprintf(stderr, "Create process failed");
        return ;
    }

    // parent will wait for the child to complete
    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("Child complete");

    // close handle
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
void hashTest()
{
    std::hash_map<std::string, std::string> hash;
}
int main()
{
    process();
    //std::thread t1(testFunc1);// t1 starts running
    //t1.join(); // main thread waits for t1 to finish
    //t1.detach(); // t1 will freely on its own -- daemon process �ػ����� 
    // ���ܱ�ϵͳ������Դ Ҳ�������е�ϵͳshut down
    // һ���߳�ֻ�ܱ�join��detach��һ��

    // �����t1������֮ǰû��join��detach,����׳��쳣

    //if(t1.joinable())
    //t1.join();

    // if (_Thr._Id == _Thrd_id()) 
    //_Throw_Cpp_error(_RESOURCE_DEADLOCK_WOULD_OCCUR);

    //int i = std::numeric_limits<int>::max();
    //while (i > 0) --i;
    //t1.join();
    return 0;
}
