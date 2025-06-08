#include "../../includes/core/MatchingEngine.h"
#include "../../includes/core/OrderBook.h"
#include <iostream>
#include <iterator>
#include <sstream>

MatchingEngine::MatchingEngine() {}

void MatchingEngine::setOutputStream(std::ostream *output_stream) {
  logger.setOutputStream(output_stream);
}

bool MatchingEngine::submitOrder(Order &order) {
  OrderBook &book = books[order.instrument];

  // Store the order first so we have a stable reference
  orders[order.order_id] = order;
  Order* orderPtr = &orders[order.order_id];

  *orderPtr = matchOrder(*orderPtr, book);

  // If the order isn't fully executed and there's remaining quantity,
  // add it to the book and log its PENDING status
  if (orderPtr->type == OrderType::LIMIT &&
      orderPtr->quantity - orderPtr->executed_quantity > 0) {
    orderPtr->status = OrderStatus::PENDING;
    book.addOrder(orderPtr);
    logger.logOrderPending(*orderPtr, orderPtr->timestamp);
  }

  return true;
}

Order MatchingEngine::matchOrder(Order &order, OrderBook &book) {
  while (order.quantity - order.executed_quantity > 0) {
    Order* bestOrder = nullptr;
    if (order.side == OrderSide::BUY && !book.asks.empty()) {
      bestOrder = book.getBestAsk();
    } else if (order.side == OrderSide::SELL && !book.bids.empty()) {
      bestOrder = book.getBestBid();
    }

    if (!bestOrder || bestOrder->price == 0) {
      break;
    }

    if (order.AcceptPrice(bestOrder->price) &&
        bestOrder->quantity - bestOrder->executed_quantity > 0) {
      int quantity = std::min(bestOrder->quantity - bestOrder->executed_quantity,
                            order.quantity - order.executed_quantity);
      executeOrder(order, quantity, bestOrder->price, bestOrder->order_id);
      executeOrder(*bestOrder, quantity, bestOrder->price, order.order_id);
    } else {
      break;
    }
  }
  return order;
}

void MatchingEngine::executeOrder(Order &order, int quantity, double price, long long counterparty_id) {
  order.executed_quantity += quantity;
  order.sum_execution_price += price * quantity;
  order.execution_price = order.sum_execution_price / order.executed_quantity;

  if (order.executed_quantity == order.quantity) {
    order.status = OrderStatus::EXECUTED;
    logger.logOrderExecution(order, quantity, price, counterparty_id, order.timestamp);
    books[order.instrument].removeOrder(order);
  } else {
    order.status = OrderStatus::PARTIALLY_EXECUTED;
    logger.logOrderPartialExecution(order, quantity, price, counterparty_id, order.timestamp);
  }
}

bool MatchingEngine::modifyOrder(Order &modifiedOrder) {
  auto it = orders.find(modifiedOrder.order_id);
  if (it == orders.end()) {
    return false;
  }

  Order &originalOrder = it->second;

  if (originalOrder.type == OrderType::MARKET) {
    return false;
  }

  if (originalOrder.status == OrderStatus::EXECUTED ||
      originalOrder.status == OrderStatus::CANCELED) {
    return false;
  }

  OrderBook &book = books[originalOrder.instrument];

  if (modifiedOrder.price != originalOrder.price) {
    book.removeOrder(originalOrder);
    originalOrder.price = modifiedOrder.price;
    originalOrder = matchOrder(originalOrder, book);
    
    if (originalOrder.quantity - originalOrder.executed_quantity > 0) {
      book.addOrder(&originalOrder);
    }
  }

  if (modifiedOrder.quantity != originalOrder.quantity) {
    if (modifiedOrder.quantity - originalOrder.executed_quantity >= 0) {
      originalOrder.quantity = modifiedOrder.quantity;
    } else {
      return false;
    }
  }

  return true;
}

bool MatchingEngine::cancelOrder(Order &cancelledOrder) {
  auto it = orders.find(cancelledOrder.order_id);
  if (it == orders.end()) {
    return false;
  }

  Order &originalOrder = it->second;

  if (originalOrder.status == OrderStatus::EXECUTED ||
      originalOrder.status == OrderStatus::CANCELED) {
    return false;
  }

  if (originalOrder.type == OrderType::MARKET) {
    return false;
  }

  books[originalOrder.instrument].removeOrder(originalOrder);
  originalOrder.status = OrderStatus::CANCELED;
  logger.logOrderCancellation(originalOrder, originalOrder.timestamp);
  return true;
}

OrderBook MatchingEngine::getOrderBook(const std::string &instrument) {
  return books[instrument];
}
