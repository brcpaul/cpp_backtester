#include "../../includes/core/MatchingEngine.h"
#include "../../includes/core/OrderBook.h"
#include <iostream>
#include <sstream>

MatchingEngine::MatchingEngine() {}

bool MatchingEngine::submitOrder(Order &order) {
  OrderBook &book = books[order.instrument];

  order = matchOrder(order, book);

  return true;
}

Order MatchingEngine::matchOrder(Order &order, OrderBook &book) {
  while (order.quantity > 0) {
    Order bestOrder;
    if (order.side == OrderSide::BUY) {
      bestOrder = book.getBestBid();
    } else {
      bestOrder = book.getBestAsk();
    }

    if (order.AcceptPrice(bestOrder.price)) {
      int quantity = std::min(bestOrder.quantity - bestOrder.executed_quantity,
                              order.quantity);
      order.quantity -= quantity;
      bestOrder.quantity -= quantity;
    }
    return order;
  }
}

void MatchingEngine::executeOrder(Order &order, int quantity, double price) {
  order.executed_quantity += quantity;
  order.sum_execution_price += price * quantity;
  order.execution_price = order.sum_execution_price / order.executed_quantity;
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