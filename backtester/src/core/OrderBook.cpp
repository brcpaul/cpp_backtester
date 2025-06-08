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


bool OrderBook::modifyOrder(const Order &modifiedOrder) {
  // Chercher l'ordre selon le côté
  if (modifiedOrder.side == OrderSide::BUY) {
    // Chercher dans les bids
    for (auto &[price, ordersList] : bids) {
      for (auto it = ordersList.begin(); it != ordersList.end(); ++it) {
        if (it->order_id == modifiedOrder.order_id) {
          // Ordre trouvé, le supprimer de sa position actuelle
          ordersList.erase(it);
          
          // Si la liste devient vide, supprimer l'entrée de prix
          if (ordersList.empty()) {
            bids.erase(price);
          }
          
          // Ajouter l'ordre modifié à sa nouvelle position
          addOrder(modifiedOrder);
          return true;
        }
      }
    }
  } else {
    // Chercher dans les asks
    for (auto &[price, ordersList] : asks) {
      for (auto it = ordersList.begin(); it != ordersList.end(); ++it) {
        if (it->order_id == modifiedOrder.order_id) {
          // Ordre trouvé, le supprimer de sa position actuelle
          ordersList.erase(it);
          
          // Si la liste devient vide, supprimer l'entrée de prix
          if (ordersList.empty()) {
            asks.erase(price);
          }
          
          // Ajouter l'ordre modifié à sa nouvelle position
          addOrder(modifiedOrder);
          return true;
        }
      }
    }
  }
  
  return false; // Ordre non trouvé
}

bool OrderBook::cancelOrder(const Order &cancelledOrder) {
  // Chercher l'ordre selon le côté
  if (cancelledOrder.side == OrderSide::BUY) {
    // Chercher dans les bids
    for (auto &[price, ordersList] : bids) {
      for (auto it = ordersList.begin(); it != ordersList.end(); ++it) {
        if (it->order_id == cancelledOrder.order_id) {
          // Ordre trouvé, le supprimer
          ordersList.erase(it);
          
          // Si la liste devient vide, supprimer l'entrée de prix
          if (ordersList.empty()) {
            bids.erase(price);
          }
          
          return true; // Ordre supprimé avec succès
        }
      }
    }
  } else {
    // Chercher dans les asks
    for (auto &[price, ordersList] : asks) {
      for (auto it = ordersList.begin(); it != ordersList.end(); ++it) {
        if (it->order_id == cancelledOrder.order_id) {
          // Ordre trouvé, le supprimer
          ordersList.erase(it);
          
          // Si la liste devient vide, supprimer l'entrée de prix
          if (ordersList.empty()) {
            asks.erase(price);
          }
          
          return true; // Ordre supprimé avec succès
        }
      }
    }
  }
  
  return false; // Ordre non trouvé
}

std::string OrderBook::toString() const {
  std::ostringstream oss;
  oss << "OrderBook:\n";

  oss << "  Bids :\n";
  for (const auto &[price, orders] : bids) {
    for (const Order &order : orders) {
      oss << "    [BUY] ID: " << order.order_id << ", Price: " << price
          << ", Qty: " << order.quantity - order.executed_quantity << "\n";
    }
  }

  oss << "  Asks :\n";
  for (const auto &[price, orders] : asks) {
    for (const auto &order : orders) {
      oss << "    [SELL] ID: " << order.order_id << ", Price: " << price
          << ", Qty: " << order.quantity - order.executed_quantity << "\n";
    }
  }

  return oss.str();
}

Order OrderBook::getBestBid() const { 
  if (bids.empty()) {
    return Order(); // Retourne un ordre par défaut
  }
  
  return bids.rbegin()->second.front(); 
}

Order OrderBook::getBestAsk() const { 
  if (asks.empty()) {
    return Order(); // Retourne un ordre par défaut
  }
  return asks.begin()->second.front(); 
}