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