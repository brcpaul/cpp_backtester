#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "Order.h"
#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include "BookSide.h"


class OrderBook {
public:
  OrderBook();

  // Ajoute un ordre
  void addOrder(const Order &order);

  // Modifie un ordre existant
  bool modifyOrder(const Order &modifiedOrder);

  // Annule un ordre
  bool cancelOrder(const Order &cancelledOrder);

  // Retourne une représentation textuelle du carnet
  std::string toString() const;

  Order getBestBid() const;
  Order getBestAsk() const;

  // Carnet structuré par prix, avec FIFO à chaque niveau
  BookSide<std::greater<double> > bids; // plus haut prix en haut
  BookSide<std::less<double> > asks; // plus bas prix en haut

private:
  // Accès rapide à un ordre pour modification/annulation
  std::unordered_map<long long, std::pair<OrderSide, OrderList::iterator> >
      orderIndex;
};

#endif // ORDER_BOOK_H
