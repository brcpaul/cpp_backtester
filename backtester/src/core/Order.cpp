#include "Order.h"

Order::Order()
    : order_id(0), instrument(""), side(OrderSide::BUY), type(OrderType::LIMIT),
      price(0.0), quantity(0), timestamp(0) {}

Order::Order(long long id, const std::string& instr, OrderSide s, OrderType t, double p, int q, long long ts)
    : order_id(id), instrument(instr), side(s), type(t), price(p), quantity(q), timestamp(ts) {}