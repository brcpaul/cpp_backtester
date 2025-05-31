#ifndef ORDER_H
#define ORDER_H

#include <string>

enum class OrderSide {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET
};

enum class OrderStatus {
    EXECUTED,
    PARTIALLY_EXECUTED,
    PENDING,
    CANCELED,
    REJECTED
};

struct Order {
    long long order_id;
    std::string instrument;
    OrderSide side;
    OrderType type;
    double price;
    int quantity;
    long long timestamp;
    
    OrderStatus status;
    double executed_quantity;
    double execution_price;
    std::string counterparty_id;
    
    Order();
    Order(long long id, const std::string& instr, OrderSide s, OrderType t, double p, int q, long long ts);
};

#endif // ORDER_H
