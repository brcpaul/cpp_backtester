#ifndef BOOK_SIDE_H
#define BOOK_SIDE_H

#include "Order.h"
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

using OrderList = std::list<Order>;

template<typename Compare = std::less<double> >
class BookSide {
public:
    BookSide();
    ~BookSide();

    bool comparePrices(double price1, double price2) const {
        return comparator(price1, price2);
    }

private:
    std::map<double, OrderList, Compare> orders;
    Compare comparator;
};

#endif // BOOK_SIDE_H