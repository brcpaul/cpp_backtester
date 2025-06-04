#include "../../includes/core/Order.h"

Order::Order()
    : order_id(0), instrument(""), side(OrderSide::BUY), type(OrderType::LIMIT),
      price(0.0), quantity(0), timestamp(0), executed_quantity(0),
      execution_price(0.0), sum_execution_price(0.0) {}

Order::Order(long long id, const std::string &instr, OrderSide s, OrderType t,
             double p, int q, long long ts, int executed_quantity,
             double execution_price, double sum_execution_price)
    : order_id(id), instrument(instr), side(s), type(t), price(p), quantity(q),
      timestamp(ts), executed_quantity(executed_quantity),
      execution_price(execution_price),
      sum_execution_price(sum_execution_price) {}

std::ostream &operator<<(std::ostream &os, OrderSide side) {
  return os << (side == OrderSide::BUY ? "BUY" : "SELL");
}

std::ostream &operator<<(std::ostream &os, OrderType type) {
  return os << (type == OrderType::LIMIT ? "LIMIT" : "MARKET");
}

std::ostream &operator<<(std::ostream &os, OrderStatus status) {
  switch (status) {
  case OrderStatus::EXECUTED:
    return os << "EXECUTED";
  case OrderStatus::PARTIALLY_EXECUTED:
    return os << "PARTIALLY_EXECUTED";
  case OrderStatus::PENDING:
    return os << "PENDING";
  case OrderStatus::CANCELED:
    return os << "CANCELED";
  case OrderStatus::REJECTED:
    return os << "REJECTED";
  }
  return os;
}

bool Order::AcceptPrice(double bestProposedPrice) {
  if (type == OrderType::LIMIT) {
    if (side == OrderSide::BUY) {
      return bestProposedPrice <= price;
    } else {
      return bestProposedPrice >= price;
    }
  } else {
    return true;
  }
}