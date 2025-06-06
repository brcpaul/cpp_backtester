#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"
#include "../includes/data/CSVReader.h"
#include "../includes/data/DataLoader.h"
#include "../includes/utils/Logger.h"

#include <iostream>

int main() {

  // 1. Charger les données CSV
  DataLoader loader("data/orders.csv");
  std::vector<Data> inputData = loader.loadData();

  std::ofstream fileOutput;
  fileOutput.open("output.csv");

  MatchingEngine engine;

  engine.setOutputStream(&fileOutput);

  // 3. On traite chaque ligne du fichier CSV
  for (const auto &data : inputData) {

    // Convertir string en long long pour l’ID
    long long orderId = std::stoll(data.order_id);

    // Convertir Side en OrderSide
    OrderSide orderSide =
        (data.side == Side::BUY) ? OrderSide::BUY : OrderSide::SELL;

    // On créé l’ordre
    Order order;
    order = Order(orderId, data.instrument, orderSide, data.type, data.price,
                  static_cast<int>(data.quantity), data.timestamp);

    // Méthodes en fonction de l’action
    if (data.action == Action::NEW) {
      engine.submitOrder(order);
    } else if (data.action == Action::MODIFY) {
      engine.modifyOrder(order);
    } else if (data.action == Action::CANCEL) {
      engine.cancelOrder(order);
    }
  }

  // 5. Etat final du carnet d’ordres pour debugger
  std::cout << engine.getOrderBook("AAPL").toString() << std::endl;
}