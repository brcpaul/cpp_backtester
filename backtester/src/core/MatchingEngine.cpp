#include "../../includes/core/MatchingEngine.h"
#include "../../includes/core/OrderBook.h"
#include <iostream>
#include <sstream>

MatchingEngine::MatchingEngine() {}

bool MatchingEngine::submitOrder(Order &order) {
  OrderBook &book = books[order.instrument];

  order = matchOrder(order, book);

  if (order.type == OrderType::LIMIT && order.quantity > 0) {
    book.addOrder(order);
  }

  return true;
}

Order MatchingEngine::matchOrder(Order &order, OrderBook &book) {
  while (order.quantity - order.executed_quantity > 0) {
    Order bestOrder;
    if (order.side == OrderSide::BUY && !book.asks.empty()) {
      bestOrder = book.getBestAsk();
    } else if (order.side == OrderSide::SELL && !book.bids.empty()) {
      bestOrder = book.getBestBid();
    }

    if (order.AcceptPrice(bestOrder.price) &&
        bestOrder.quantity - bestOrder.executed_quantity > 0) {
      int quantity = std::min(bestOrder.quantity - bestOrder.executed_quantity,
                              order.quantity - order.executed_quantity);
      executeOrder(order, quantity, bestOrder.price);
      executeOrder(bestOrder, quantity, bestOrder.price);

      if (bestOrder.quantity - bestOrder.executed_quantity == 0) {
        if (order.side == OrderSide::BUY) {
          book.asks[bestOrder.price].pop_front();
          if (book.asks[bestOrder.price].empty()) {
            book.asks.erase(bestOrder.price);
          }
        } else {
          book.bids[bestOrder.price].pop_front();
          if (book.bids[bestOrder.price].empty()) {
            book.bids.erase(bestOrder.price);
          }
        }
      }
    }
    return order;
  }
}

void MatchingEngine::executeOrder(Order &order, int quantity, double price) {
  order.executed_quantity += quantity;
  order.sum_execution_price += price * quantity;
  order.execution_price = order.sum_execution_price / order.executed_quantity;

  if (order.executed_quantity == order.quantity) {
    order.status = OrderStatus::EXECUTED;
    logOrderExecution(order, order.timestamp);
  } else {
    order.status = OrderStatus::PARTIALLY_EXECUTED;
    logOrderPartialExecution(order, quantity, price, order.timestamp);
  }
}

bool MatchingEngine::modifyOrder(Order &modifiedOrder) { 

  if(modifiedOrder.type == OrderType::MARKET) {
    return false;
  }
  

}

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

void MatchingEngine::logOrderExecution(const Order &order,
                                       long long timestamp) {
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.price << ","
      << order.quantity << ",EXECUTED," << order.executed_quantity << ","
      << order.execution_price << "," << order.execution_price;
  std::cout << oss.str() << std::endl;
}

void MatchingEngine::logOrderPartialExecution(const Order &order,
                                              int executed_quantity,
                                              double execution_price,
                                              long long timestamp) {
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.price << ","
      << order.quantity << ",PARTIALLY_EXECUTED," << executed_quantity << ","
      << execution_price << "," << order.execution_price;
  std::cout << oss.str() << std::endl;
}