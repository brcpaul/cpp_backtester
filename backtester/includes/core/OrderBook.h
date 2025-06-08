#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "Order.h"
#include <list>
#include <map>
#include <string>
// #include <unordered_map>
#include <vector>

using OrderList = std::list<Order>;

class OrderBook {
public:
  OrderBook();

  // Ajoute un ordre
  void addOrder(Order* order);

  // Retire un ordre
  void removeOrder(const Order& order);

  // Retourne une représentation textuelle du carnet
  std::string toString() const;

  Order* getBestBid() const;
  Order* getBestAsk() const;

  // Carnet structuré par prix, avec FIFO à chaque niveau
  std::map<double, std::list<Order*>, std::greater<double> > bids; // plus haut prix en haut
  std::map<double, std::list<Order*> > asks; // plus bas prix en haut
};

#endif // ORDER_BOOK_H
