#include "../../includes/core/MatchingEngine.h"
#include <iostream>
#include <sstream>

MatchingEngine::MatchingEngine() {}

bool MatchingEngine::submitOrder(Order &order) {
    OrderBook &book = books[order.instrument];
    
}

bool MatchingEngine::modifyOrder(Order &modifiedOrder) { return true; }

bool MatchingEngine::cancelOrder(Order &cancelledOrder) { return true; }

OrderBook MatchingEngine::getOrderBook(const std::string &instrument) {
  return books[instrument];
}

void MatchingEngine::logNewOrder(const Order &order, long long timestamp) {
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.price << ","
      << order.quantity << "," << order.status << ",0,0,0";
  std::cout << oss.str() << std::endl;
}