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
    virtual ~Quote() = default;
private: 
    std::string bookNo;
protected:
    double price = 0.0;
};

class Bulk_quote final: public Quote 
{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string& book, double sales_price, std::size_t qty, double disc) : 
        Quote(book, sales_price), min_qty(qty), discount(disc) { }
    /* virtual <optional>*/double net_price( std::size_t n) const override
    {
        if( n > min_qty)
            return n * (1 - discount) * price;
        else
            return n * price;
    }
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

class t : Bulk_quote
{

};

double print_total(std::ostream &os, const Quote& item, std::size_t m)
{
    double ret = item.net_price(m);
    os << "ISBN: " << item.isbn()
       << " # sold: " << m << " total due: " << ret << std::endl;
    return ret;
}

int main()
{
    Bulk_quote bq;
    Quote* p = &bq;
    Quote& r = bq; 
}
 
