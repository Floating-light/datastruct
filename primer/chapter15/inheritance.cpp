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

class Bulk_quote : public Quote 
{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string&, double, std::size_t, double);
    /* virtual <optional>*/double net_price( std::size_t n) const override;
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

double print_total(std::ostream &os, const Quote& item, std::size_t m)
{
    double ret = item.net_price(m);
    os << "ISBN: " << item.isbn()
       << " # sold: " << m << " total due: " << ret << std::endl;
    return ret;
}
 
