// The effect of specifiers override
struct B
{
    virtual void f1( int ) const;
    virtual void f2() ;
    void f3();
};

struct D : B
{
    void f1( int ) const override;
    void f2(int) override;
    void f3() override;
    void f4() override;
};

struct D1 : B
{
    void f1( int ) const ;
    void f2(int) ; // a new function in D1, always be a bug.
    void f3() ;
    void f4() ;
};

struct D2 : B
{
    void f1( int ) const final override;
};

struct D3 : D2
{
    void f1( int) const ;
};