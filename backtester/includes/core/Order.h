#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>

enum OrderSide { BUY, SELL };

std::ostream &operator<<(std::ostream &os, OrderSide side);

enum OrderType { LIMIT, MARKET };

std::ostream &operator<<(std::ostream &os, OrderType type);

enum OrderStatus { EXECUTED, PARTIALLY_EXECUTED, PENDING, CANCELED, REJECTED };

std::ostream &operator<<(std::ostream &os, OrderStatus status);

class Order {
public:
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

  bool AcceptPrice(double bestProposedPrice);

  Order();
  Order(long long id, const std::string &instr, OrderSide s, OrderType t,
        double p, int q, long long ts, int executed_quantity = 0,
        double execution_price = 0.0, double sum_execution_price = 0.0);
};

#endif // ORDER_H
