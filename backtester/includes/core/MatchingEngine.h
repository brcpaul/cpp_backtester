#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include "../utils/Logger.h"
#include "Order.h"
#include "OrderBook.h"
#include <string>
#include <unordered_map>
#include <vector>

class MatchingEngine {
public:
  MatchingEngine();

  // Crée ou route un ordre vers le carnet correspondant
  bool submitOrder(Order &order);

  // Modifie un ordre existant
  bool modifyOrder(Order &modifiedOrder);

  // Annule un ordre
  bool cancelOrder(Order &cancelledOrder);

  OrderBook getOrderBook(const std::string &instrument);

  void setOutputStream(std::ostream *output_stream);

private:
  // Chaque instrument a son OrderBook
  std::unordered_map<std::string, OrderBook> books;

  std::unordered_map<long, Order> orders;

  Logger logger;

  Order matchOrder(Order &order, OrderBook &book);

  void executeOrder(Order &order, int quantity, double price);
};

#endif // MATCHING_ENGINE_H
