
class base{
public:
    int i;
};

class D1 : base // default private derivation specifier
{
    int cla; // default private
};

struct D2 : base // default public derivation specifier
{
    int str; // default public
};

int main()
{
    D1 d1;
    d1.i; // inaccessible

    D2 d2;
    d2.i;
}
