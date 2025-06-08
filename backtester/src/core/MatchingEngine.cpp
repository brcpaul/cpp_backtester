#include "../../includes/core/MatchingEngine.h"
#include "../../includes/core/OrderBook.h"
#include <iostream>
#include <iterator>
#include <sstream>

MatchingEngine::MatchingEngine() {}

void MatchingEngine::setOutputStream(std::ostream *output_stream) {
  logger.setOutputStream(output_stream);
}

bool MatchingEngine::submitOrder(Order &order) {
  OrderBook &book = books[order.instrument];

  order = matchOrder(order, book);

  // Si l'ordre n'est pas complètement exécuté et qu'il reste une quantité, 
  // on l'ajoute au carnet et on log son statut PENDING
  if (order.type == OrderType::LIMIT && order.quantity - order.executed_quantity > 0) {
    order.status = OrderStatus::PENDING;
    book.addOrder(order);
    logger.logOrderPending(order, order.timestamp);
  }

  orders[order.order_id] = order;

  return true;
}

Order MatchingEngine::matchOrder(Order &order, OrderBook &book) {
  while (order.quantity - order.executed_quantity > 0) {
    Order bestOrder;
    
    if (order.side == OrderSide::BUY && !book.asks.empty()) {
      bestOrder = book.getBestAsk();
    } else if (order.side == OrderSide::SELL && !book.bids.empty()) {
      bestOrder = book.getBestBid();
    } else {
      // Pas de contrepartie disponible
      break;
    }
    
    // Si prix non acceptable ou pas de quantité disponible, sortir de la boucle
    if (!order.AcceptPrice(bestOrder.price) || 
        bestOrder.quantity - bestOrder.executed_quantity <= 0) {
      break;
    }

    // Calculer la quantité à exécuter
    int quantity = std::min(bestOrder.quantity - bestOrder.executed_quantity,
                            order.quantity - order.executed_quantity);
    
    // Exécuter les deux ordres
    executeOrder(order, quantity, bestOrder.price, bestOrder.order_id);
    executeOrder(bestOrder, quantity, bestOrder.price, order.order_id);

    // Mettre à jour l'ordre dans le carnet d'ordres
    if (order.side == OrderSide::BUY && !book.asks[bestOrder.price].empty()) {
      // Mettre à jour le premier ordre de la liste des asks
      book.asks[bestOrder.price].front().executed_quantity = bestOrder.executed_quantity;
      book.asks[bestOrder.price].front().execution_price = bestOrder.execution_price;
      book.asks[bestOrder.price].front().sum_execution_price = bestOrder.sum_execution_price;
      book.asks[bestOrder.price].front().status = bestOrder.status;
      
      // Si complètement exécuté, retirer de la liste
      if (book.asks[bestOrder.price].front().quantity - book.asks[bestOrder.price].front().executed_quantity == 0) {
        book.asks[bestOrder.price].pop_front();
        if (book.asks[bestOrder.price].empty()) {
          book.asks.erase(bestOrder.price);
        }
      }
    } else if (order.side == OrderSide::SELL && !book.bids[bestOrder.price].empty()) {
      // Mettre à jour le premier ordre de la liste des bids
      book.bids[bestOrder.price].front().executed_quantity = bestOrder.executed_quantity;
      book.bids[bestOrder.price].front().execution_price = bestOrder.execution_price;
      book.bids[bestOrder.price].front().sum_execution_price = bestOrder.sum_execution_price;
      book.bids[bestOrder.price].front().status = bestOrder.status;
      
      // Si complètement exécuté, retirer de la liste
      if (book.bids[bestOrder.price].front().quantity - book.bids[bestOrder.price].front().executed_quantity == 0) {
        book.bids[bestOrder.price].pop_front();
        if (book.bids[bestOrder.price].empty()) {
          book.bids.erase(bestOrder.price);
        }
      }
    }
    
    // Mettre à jour l'ordre dans la map des ordres
    orders[bestOrder.order_id] = bestOrder;
  }
  
  return order;
}

void MatchingEngine::executeOrder(Order &order, int quantity, double price, long long counterparty_order_id) {
  order.executed_quantity += quantity;
  order.sum_execution_price += price * quantity;
  order.execution_price = order.sum_execution_price / order.executed_quantity;

  if (order.executed_quantity == order.quantity) {
    order.status = OrderStatus::EXECUTED;
    logger.logOrderExecution(order, order.timestamp, counterparty_order_id);
  } else {
    order.status = OrderStatus::PARTIALLY_EXECUTED;
    logger.logOrderPartialExecution(order, quantity, price, order.timestamp, counterparty_order_id);
  }
}

bool MatchingEngine::modifyOrder(Order &modifiedOrder) {
  // Vérifier si l'ordre existe
  if (orders.find(modifiedOrder.order_id) == orders.end()) {
    logger.logOrderRejection(modifiedOrder, modifiedOrder.timestamp, "Modification rejected: Order not found");
    return false;
  }

  Order &originalOrder = orders[modifiedOrder.order_id];

  // Vérifier si l'ordre peut être modifié
  if (originalOrder.type == OrderType::MARKET) {
    logger.logOrderRejection(modifiedOrder, modifiedOrder.timestamp, "Modification rejected: Cannot modify market order");
    return false;
  }

  if (originalOrder.status == OrderStatus::EXECUTED ||
      originalOrder.status == OrderStatus::CANCELED) {
    logger.logOrderRejection(modifiedOrder, modifiedOrder.timestamp, "Modification rejected: Order already executed or canceled");
    return false;
  }

  // Vérifier que la nouvelle quantité est au moins égale à la quantité déjà exécutée
  if (modifiedOrder.quantity < originalOrder.executed_quantity) {
    logger.logOrderRejection(modifiedOrder, modifiedOrder.timestamp, "Modification rejected: New quantity cannot be less than executed quantity");
    return false;
  }

  OrderBook &book = books[originalOrder.instrument];

  // Créer une copie de l'ordre original pour la modification
  Order orderToModify = originalOrder;
  orderToModify.price = modifiedOrder.price;
  orderToModify.quantity = modifiedOrder.quantity;

  // Utiliser la méthode modifyOrder de l'OrderBook
  bool success = book.modifyOrder(orderToModify);
  
  if (success) {
    // Log de la modification
    logger.logOrderModification(orderToModify, orderToModify.timestamp);

    // Mettre à jour l'ordre dans la map des ordres
    orders[modifiedOrder.order_id] = orderToModify;
    
    // Essayer de matcher l'ordre modifié
    Order matchedOrder = matchOrder(orderToModify, book);
    
    // Si l'ordre n'est pas complètement exécuté, l'ajouter au carnet
    if (matchedOrder.type == OrderType::LIMIT && 
        matchedOrder.quantity - matchedOrder.executed_quantity > 0) {
      matchedOrder.status = OrderStatus::PENDING;
      book.addOrder(matchedOrder);
    }
    
    // Mettre à jour l'ordre final
    orders[modifiedOrder.order_id] = matchedOrder;
  }

  return success;
}

bool MatchingEngine::cancelOrder(Order &cancelledOrder) {

  // Vérifier si l'ordre existe
  if (orders.find(cancelledOrder.order_id) == orders.end()) {
    logger.logOrderRejection(cancelledOrder, cancelledOrder.timestamp, "Cancellation rejected: Order not found");
    return false;
  }

  Order &originalOrder = orders[cancelledOrder.order_id];

  // Vérifier si l'ordre peut être annulé
  if (originalOrder.status == OrderStatus::EXECUTED ||
      originalOrder.status == OrderStatus::CANCELED) {
    logger.logOrderRejection(cancelledOrder, cancelledOrder.timestamp, "Cancellation rejected: Order already executed or canceled");
    return false;
  }

  if (originalOrder.type == OrderType::MARKET) {
    logger.logOrderRejection(cancelledOrder, cancelledOrder.timestamp, "Cancellation rejected: Cannot cancel market order");
    return false;
  }

  OrderBook &book = books[originalOrder.instrument];
  
  // Mettre à jour le timestamp de l'ordre original
  originalOrder.timestamp = cancelledOrder.timestamp;
  
  // Utiliser la méthode cancelOrder de l'OrderBook
  bool success = book.cancelOrder(originalOrder);
  
  if (success) {
    originalOrder.status = OrderStatus::CANCELED;
    orders[cancelledOrder.order_id] = originalOrder;
    logger.logOrderCancellation(originalOrder, originalOrder.timestamp);
  } else {
    logger.logOrderRejection(cancelledOrder, cancelledOrder.timestamp, "Failed to cancel order in book");
  }

  return success;
}

OrderBook MatchingEngine::getOrderBook(const std::string &instrument) {
  return books[instrument];
}
