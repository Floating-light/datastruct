#pragma once
#include <iostream>
class NoCopy
{
    public:
    NoCopy() = default;
    NoCopy(const NoCopy& ) = delete;
    ~NoCopy() = default;

    NoCopy& operator=(const NoCopy&) = delete;
};

class NoDesturctor
{
public:
    NoDesturctor() = default;
    ~NoDesturctor() = delete;//can not define a variable of such type.
                             //but can dtnamically allocate objects with a deleted destructor.
                             //also can not release it.
};

//NoDesturctor nd;
NoDesturctor* p = new NoDesturctor();
//delete p;

class ConstMember
{
    public:
    ConstMember(int& a ):ii(a)
    {

    }
    ~ConstMember() = default;

    int& ii ;//that is OK.
    //const int ii;// cause default constrcutor be deleteed.
    //int& a;//this also.
           //besause them can not be default construct. 
};

//extern
//***private Copy Control***
//prior to the new standard(c++11), classes prevented copies by declaring 
//their copy constructor and copy-assignment operator as private.
class PrivateCopy
{
    //copy constrol is private and so is inaccessible to ordinary user code. 
    PrivateCopy(const PrivateCopy&);
    PrivateCopy& operator=(const PrivateCopy&);
    //to prevent copies by friends and members, we declared
public:
    PrivateCopy() = default;
    ~PrivateCopy();
};
int main()
{
    int p = 111;
    ConstMember c(p);
    int q = 0;
    ConstMember d(q);
    std::cout << d.ii << std::endl;
    std::cout << c.ii << std::endl;
    
    //d = c;//error, implicitly deleted because of reference member.
}

//If a class has a data member that cannot be default constructed, copied, assigned, 
//or destroyed, then the corresponding member will be a deleted function.
