#include "../../includes/core/MatchingEngine.h"
#include <cassert>
#include <iostream>

void testSubmitOrder() {
    MatchingEngine engine;
    Order order(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    bool result = engine.submitOrder(order);
    assert(result);
    OrderBook ob = engine.getOrderBook("AAPL");
    assert(!ob.bids.empty());
    assert(ob.bids[100.0].front()->order_id == 1);
}

void testOrderMatching() {
    MatchingEngine engine;
    Order buy(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    Order sell(2, "AAPL", SELL, LIMIT, 100.0, 10, 1);
    engine.submitOrder(buy);
    engine.submitOrder(sell);
    OrderBook ob = engine.getOrderBook("AAPL");
    assert(ob.bids.empty());
    assert(ob.asks.empty());
}

void testModifyOrder() {
    MatchingEngine engine;
    Order order(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    engine.submitOrder(order);
    Order modified(1, "AAPL", BUY, LIMIT, 101.0, 10, 0);
    bool result = engine.modifyOrder(modified);
    assert(result);
    OrderBook ob = engine.getOrderBook("AAPL");
    assert(ob.bids.find(101.0) != ob.bids.end());
}

void testCancelOrder() {
    MatchingEngine engine;
    Order order(1, "AAPL", BUY, LIMIT, 100.0, 10, 0);
    engine.submitOrder(order);
    bool result = engine.cancelOrder(order);
    assert(result);
    OrderBook ob = engine.getOrderBook("AAPL");
    assert(ob.bids.empty());
}

int main() {
    testSubmitOrder();
    testOrderMatching();
    testModifyOrder();
    testCancelOrder();
    std::cout << "All MatchingEngine tests passed!\n";
    return 0;
}
