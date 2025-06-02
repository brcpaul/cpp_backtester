#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>

enum class OrderSide { BUY, SELL };

std::ostream &operator<<(std::ostream &os, OrderSide side);

enum class OrderType { LIMIT, MARKET };

std::ostream &operator<<(std::ostream &os, OrderType type);

enum OrderStatus { EXECUTED, PARTIALLY_EXECUTED, PENDING, CANCELED, REJECTED };

std::ostream &operator<<(std::ostream &os, OrderStatus status);

struct Order {
  long long order_id;
  std::string instrument;
  OrderSide side;
  OrderType type;
  double price;
  int quantity;
  long long timestamp;

  OrderStatus status;
  int executed_quantity;
  double execution_price;
  double sum_execution_price;

  bool AcceptPrice(int price);

  Order();
  Order(long long id, const std::string &instr, OrderSide s, OrderType t,
        double p, int q, long long ts);
};

#endif // ORDER_H
