#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include <unordered_map>
#include <vector>
#include <string>
#include "Order.h"
#include "OrderBook.h"

class MatchingEngine {
public:
    MatchingEngine();

    // Crée ou route un ordre vers le carnet correspondant
    bool submitOrder(Order& order);

    // Modifie un ordre existant
    bool modifyOrder(Order& modifiedOrder);

    // Annule un ordre
    bool cancelOrder(Order& cancelledOrder);

    OrderBook getOrderBook(const std::string& instrument);

private:
    // Chaque instrument a son OrderBook
    std::unordered_map<std::string, OrderBook> books;

    // Associe orderId -> instrument (permet de retrouver le bon carnet)
    std::unordered_map<long, std::string> orderIdToInstrument;

    Order matchOrder(Order& order, OrderBook& book);

    void executeOrder(Order& order, int quantity, double price);

    void logNewOrder(const Order& order, long long timestamp);
    void logOrderExecution(const Order& order, double executed_quantity, double execution_price, long long timestamp);
    void logOrderPartialExecution(const Order& order, double executed_quantity, double execution_price, long long timestamp);
    void logOrderCancellation(const Order& order, long long timestamp); 
    void logOrderModification(const Order& order, long long timestamp);
    void logOrderRejection(const Order& order, long long timestamp);
};

#endif // MATCHING_ENGINE_H
