#include <string>
#include <vector>
#include <iostream>

class Quote 
{
public:
    Quote() = default;
    Quote(const std::string& book, double sales_price) : 
        bookNo(book), price(sales_price) {}
    std::string isbn() const
    {
        return bookNo;
    }
    virtual double net_price( std::size_t n) const
    {
        return price * n;
    }
    virtual void debug()  
    {
        std::cout << "from base quote: " << std::endl
            << "book No : " << bookNo << std::endl
            << "price : " << price << std::endl;
    }
    virtual ~Quote() = default;
private: 
    std::string bookNo;
protected:
    double price = 0.0;
};

class Disc_quote : public Quote
{
public:
    Disc_quote() = default;
    Disc_quote(const std::string& book, double price, std::size_t qty, double disc):
        Quote(book, price), quantity(qty), discount(disc) {}
    
    double net_price( std::size_t n) const = 0;
    virtual ~Disc_quote() = default;
protected:
    std::size_t quantity = 0;
    double discount = 0.0;
};

class Bulk_quote : public Disc_quote
{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string& book, double price, std::size_t qty, double disc) : 
        Disc_quote(book, price, qty, disc) { }
    
    double net_price( std::size_t n) const override
    {
        if( n > quantity)
            return price * n * discount;
        else
            return price * n;
    }
    void debug() override
    {
        std::cout << "from derived Bulk_quote: " << std::endl
            << "price : " << price << std::endl;
    }
    ~Bulk_quote() = default;
};

class Limit_quote : public Disc_quote
{
    Limit_quote() = default;
    Limit_quote(const std::string& book, double price, std::size_t qty, double disc) :
       Disc_quote(book, price, qty, disc) { }
    
    double net_price( std::size_t n) const override
    {
        if( n > quantity)
            return quantity * price + (n - quantity) * price * (1 - discount);
        else
            return n * price;
    }

    void debug() override
    {
        std::cout << "from derived Bulk_quote: " << std::endl
            << "price : " << price << std::endl;
    }

    ~Limit_quote() = default;
};

int main()
{
    Quote dq;
    dq.debug();
    Bulk_quote bq;
    bq.debug();

    Quote& qr = bq;
    qr.debug();
}
