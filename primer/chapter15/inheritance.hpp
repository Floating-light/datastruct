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
    virtual void debug() const 
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
    void debug() const override
    {
        std::cout << "from base quote: " << std::endl
            << "price : " << price << std::endl
            << "min qty : " << min_qty << std::endl
            << "discount : " << discount << std::endl;
    }
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

class Limit_quote final : public Quote
{
public:
    Limit_quote() = default;
    Limit_quote(const std::string& book, double sales_price, std::size_t qty, double disc) : 
        Quote(book, sales_price), limits(qty), discount(disc) { }
    /* virtual <optional>*/double net_price( std::size_t n) const override
    {
        if( n > limits)
            return (n - limits) * (1 - discount) * price + (n - limits) * price;
        else
            return n * price;
    }
        void debug() const override
    {
        std::cout << "from base quote: " << std::endl
            << "price : " << price << std::endl
            << "limits : " << limits << std::endl
            << "discount : " << discount << std::endl;
    }
private:
    std::size_t limits = 0;
    double discount = 0.0;
};

// int main()
// {
//     Quote base;
//     Bulk_quote& bk = base;

//     Bulk_quote bulk;
    
//     Quote ba(bulk); // call Quote::Quote(const Quote& )
//     base = bulk; //    call Quote::=(const Quote& )

//     Quote* bulkq = &bulk;
//     Bulk_quote* b = bulkq; // must use dynamic_cast<>   , check at run time
//                            // or use static_cast<> if we know this conversion is safe, to override the compiler.
// }

double print_total(std::ostream &os, const Quote& item, std::size_t m)
{
    item.debug();
    double ret = item.net_price(m);
    os << "ISBN: " << item.isbn()
       << " # sold: " << m << " total due: " << ret << std::endl;
    return ret;
}

/* int main()
{
    std::cout << "any error ? " << std::endl;
    Quote q1("121234256", 100);
    Bulk_quote bq1("8765432", 120, 5, 0.2);
    Limit_quote lq1("7654325", 144, 5, 0.2);
    print_total(std::cout, q1, 10);
    print_total(std::cout, bq1, 10);
    print_total(std::cout, lq1, 10);

} */