#include "../../includes/core/OrderBook.h"
#include <cassert>
#include <iostream>

void testAddOrder() {
    OrderBook ob;
    Order* o1 = new Order(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    ob.addOrder(o1);
    assert(ob.bids[100.0].front() == o1);
    delete o1;
}

void testRemoveOrder() {
    OrderBook ob;
    Order* o1 = new Order(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    ob.addOrder(o1);
    ob.removeOrder(*o1);
    assert(ob.bids.find(100.0) == ob.bids.end());
    delete o1;
}

void testBestBidAsk() {
    OrderBook ob;
    Order* b1 = new Order(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    Order* b2 = new Order(2, "AAPL", BUY, LIMIT, 101.0, 5, 0);
    Order* a1 = new Order(3, "AAPL", SELL, LIMIT, 102.0, 7, 0);
    ob.addOrder(b1);
    ob.addOrder(b2);
    ob.addOrder(a1);
    assert(ob.getBestBid() == b2); // highest price
    assert(ob.getBestAsk() == a1); // lowest price
    delete b1; delete b2; delete a1;
}

int runOrderBookTests() {
    testAddOrder();
    testRemoveOrder();
    testBestBidAsk();
    std::cout << "All OrderBook tests passed!\n";
    return 0;
}
