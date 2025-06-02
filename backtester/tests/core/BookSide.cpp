#include "../../includes/core/BookSide.h"
#include <cassert>

void runBookSideTests() {
    BookSide<std::greater<double> > bids;
    BookSide<std::less<double> > asks;
    assert(bids.comparePrices(10.0, 9.0));
    assert(!bids.comparePrices(9.0, 10.0));
    assert(asks.comparePrices(9.0, 10.0));
    assert(!asks.comparePrices(10.0, 9.0));
}