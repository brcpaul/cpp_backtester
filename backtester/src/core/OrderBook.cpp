#include "../../includes/core/OrderBook.h"
#include <sstream>

OrderBook::OrderBook() {}

void OrderBook::addOrder(Order* order) {
  if (order->side == OrderSide::BUY) {
    bids[order->price].push_back(order);
  } else if (order->side == OrderSide::SELL) {
    asks[order->price].push_back(order);
  }
}

void OrderBook::removeOrder(const Order& order) {
  if (order.side == OrderSide::BUY) {
    auto& orders = bids[order.price];
    orders.remove_if([&order](Order* o) { return o->order_id == order.order_id; });
    if (orders.empty()) {
      bids.erase(order.price);
    }
  } else if (order.side == OrderSide::SELL) {
    auto& orders = asks[order.price];
    orders.remove_if([&order](Order* o) { return o->order_id == order.order_id; });
    if (orders.empty()) {
      asks.erase(order.price);
    }
  }
}

std::string OrderBook::toString() const {
  std::ostringstream oss;
  oss << "OrderBook:\n";

  oss << "  Bids:\n";
  for (const auto& [price, orders] : bids) {
    for (const Order* order : orders) {
      oss << "    [BUY] ID: " << order->order_id << ", Price: " << price
          << ", Qty: " << order->quantity - order->executed_quantity << "\n";
    }
  }

  oss << "  Asks:\n";
  for (const auto& [price, orders] : asks) {
    for (const Order* order : orders) {
      oss << "    [SELL] ID: " << order->order_id << ", Price: " << price
          << ", Qty: " << order->quantity - order->executed_quantity << "\n";
    }
  }

  return oss.str();
}

Order* OrderBook::getBestBid() const {
  return !bids.empty() ? bids.begin()->second.front() : nullptr;
}

Order* OrderBook::getBestAsk() const {
  return !asks.empty() ? asks.begin()->second.front() : nullptr;
}