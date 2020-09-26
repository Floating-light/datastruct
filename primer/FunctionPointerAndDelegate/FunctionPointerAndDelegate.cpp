#include <iostream>
#include <string>

using namespace std;

class MyTest 
{
    public:
    int TestFunc1(std::string str)
    {
        int ret = 1;
        std::cout << ret << "--------->" << str << std::endl; 
        return ret;
    }

    float TestFunc2(std::string str)
    {
        int ret = 2.0;
        std::cout << ret << "--------->" << str << std::endl;
        return ret;
    }

    private:
    int i;
};

int main()
{
    MyTest* MyObj = new MyTest();
    using Func1PtrType = int (MyTest::* )(std::string );

    Func1PtrType Func1Ptr = &MyTest::TestFunc1;

    printf("===>>>%x\n", Func1Ptr);
    printf("===>>>%x\n", &MyTest::TestFunc1);

    printf("===>>>%x\n", &MyTest::TestFunc2);


    std::cout << "Func1 address: " << Func1Ptr << std::endl;
    std::cout << "Func2 address: " << &MyTest::TestFunc2 << std::endl;
    
    (MyObj->*Func1Ptr)("Call func via member function pointer ");

}