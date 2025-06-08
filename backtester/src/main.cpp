#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"
#include "../includes/data/CSVReader.h"
#include "../includes/data/DataLoader.h"
#include "../includes/utils/Logger.h"

#include <iostream>

#include "../includes/data/OrderGenerator.h"
#include <vector>
/*
int main() {
    
    OrderGenerator generator;
    
    // Nombre d'ordres à générer
    const int num_orders = 100;
    std::vector<RandomOrder> orders;
    
    // Génération des ordres
    for (int i = 0; i < num_orders; ++i) {
        orders.push_back(generator.generate_order());
    }
    
    // Export vers CSV
    generator.export_to_csv(orders, "data/random_orders.csv");
    
    return 0;
}

*/

int main() {

  // 1. Charger les données CSV
  DataLoader loader("data/orders.csv");
  // DataLoader loader("data/random_orders.csv");
  std::vector<Data> inputData = loader.loadData();

  std::ofstream fileOutput;
  fileOutput.open("output.csv");

  MatchingEngine engine;

  // engine.setOutputStream(&fileOutput);
  engine.setOutputStream(&std::cout);

  // 3. On traite chaque ligne du fichier CSV
  for (const auto &data : inputData) {

    // Convertir Side en OrderSide
    OrderSide orderSide =
        (data.side == Side::BUY) ? OrderSide::BUY : OrderSide::SELL;

    // On créé l’ordre
    Order order;
    order = Order(data.order_id, data.instrument, orderSide, data.type, data.price,
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
