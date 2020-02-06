#include <memory>
#include <set>

#include "primer/chapter15/inheritance.hpp"

class Basket 
{
    public:
    // uses synthesized default constructor and copy-control members

    void add_item(const std::shared_ptr<Quote>& sale)
    {
        items.insert(sale);
    }

    void add_item(const Quote& sale)
    {
        items.insert(std::shared_ptr<Quote>(sale.clone()));
    }

    void add_item(Quote&& sale)
    {
        items.insert(std::shared_ptr<Quote>(std::move(sale.clone())));
    }
    
    // prints the total price for each booK and the overall total for
    // all items inthe basket.
    double total_receipt(std::ostream&) const;

    private:
    static bool compare(const std::shared_ptr<Quote> &lhs,
                        const std::shared_ptr<Quote> &rhs)
                        {
                            return lhs->isbn() < rhs->isbn();
                        };
    // 保存 事务，可以多个相同。
    std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items { compare};
};

double Basket::total_receipt(std::ostream& os) const
{
    double sum = 0.0;

    for(auto itr = items.cbegin(); itr != items.cend(); items.upper_bound(*itr))
    {
        sum += print_total(os, **itr, items.count(*itr));
    }

    os << "Total Sale: " << sum << std::endl;
    return sum;
}