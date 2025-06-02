#include "../../includes/core/OrderBook.h"
#include <sstream>

OrderBook::OrderBook() {}

void OrderBook::addOrder(const Order &order) {

  if (order.side == OrderSide::BUY) {
    bids[order.price].push_back(order);
  } else if (order.side == OrderSide::SELL) {
    asks[order.price].push_back(order);
  }
}

bool OrderBook::modifyOrder(const Order &modifiedOrder) { return true; }

bool OrderBook::cancelOrder(const Order &cancelledOrder) { return true; }

std::string OrderBook::toString() const {
  std::ostringstream oss;
  oss << "OrderBook:\n";

  oss << "  Bids :\n";
  for (const auto &[price, orders] : bids) {
    for (const Order &order : orders) {
      oss << "    [BUY] ID: " << order.order_id << ", Price: " << price
          << ", Qty: " << order.quantity << "\n";
    }
  }

  oss << "  Asks :\n";
  for (const auto &[price, orders] : asks) {
    for (const auto &order : orders) {
      oss << "    [SELL] ID: " << order.order_id << ", Price: " << price
          << ", Qty: " << order.quantity << "\n";
    }
  }

  return oss.str();
}

Order OrderBook::getBestBid() const { return bids.begin()->second.front(); }

Order OrderBook::getBestAsk() const { return asks.begin()->second.front(); }