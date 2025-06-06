// #include "data/CSVWriter.h"
// #include "data/DataConverter.h"
// #include "data/DataLoader.h"

#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"

#include "data/DataLoader.h"
#include "data/DataConverter.h"
#include "data/CSVWriter.h"
#include "data/CSVReader.h"
#include "core/Order.h"         
#include "core/MatchingEngine.h"  


#include <iostream>

int main() {

      // 1. Charger les données CSV
    DataLoader loader("data/orders.csv");
    std::vector<Data> inputData = loader.loadData();

    // 2. Initialisation
    DataConverter converter;
    MatchingEngine engine;
    std::vector<OutputData> outputData;

    // 3. On traite chaque ligne du fichier CSV
    for (const auto& data : inputData) {

        // Convertir string en long long pour l’ID
        long long orderId = std::stoll(data.order_id);

        // Convertir Side en OrderSide
        OrderSide orderSide = (data.side == Side::BUY) ? OrderSide::BUY : OrderSide::SELL;

        // On créé l’ordre
        Order order;
        order = Order(
            orderId,
            data.instrument,
            orderSide,
            data.type,
            data.price,
            static_cast<int>(data.quantity),
            data.timestamp
        );
      

        // Méthodes en fonction de l’action
        if (data.action == Action::NEW) {
            engine.submitOrder(order);
        } else if (data.action == Action::MODIFY) {
            engine.modifyOrder(order);
        } else if (data.action == Action::CANCEL) {
            engine.cancelOrder(order);
        }
      

        // Enregistrer l'état après traitement
        OutputData out = converter.convertToOutput(data);
        out.status = order.status;
        out.executed_quantity = order.executed_quantity;
        out.execution_price = order.execution_price;
        out.counterparty_id = out.counterparty_id;
        // Ajout au log centralisé
        engine.logOutput(out);

        outputData.push_back(out);
       
    }

    // 4. Fichier de sortie
    CSVWriter writer("output_results.csv");
    writer.writeHeader();
    writer.writeData(engine.getLogs());
    writer.close();

    // 5. Etat final du carnet d’ordres
    std::cout << engine.getOrderBook("AAPL").toString() << std::endl;
    
  // MatchingEngine engine;

  // Order order;

  // order = Order(1, "AAPL", OrderSide::BUY, OrderType::LIMIT, 100, 2,
  //               171717171717171717);

  // engine.submitOrder(order);

  // order = Order(2, "AAPL", OrderSide::SELL, OrderType::LIMIT, 102, 2,
  //               171717171717171717);

  // engine.submitOrder(order);

  // order = Order(3, "AAPL", OrderSide::BUY, OrderType::LIMIT, 100, 2,
  //               171717171717171717);
  // engine.submitOrder(order);

  // order = Order(4, "AAPL", OrderSide::SELL, OrderType::MARKET, 103, 2,
  //               171717171717171717);
  // engine.submitOrder(order);

  // order = Order(5, "AAPL", OrderSide::SELL, OrderType::LIMIT, 99, 10,
  //               171717171717171717);
  // engine.submitOrder(order);

  // std::cout << engine.getOrderBook("AAPL").toString() << std::endl;

  


  // // 1. Charger les données d'entrée
  // DataLoader loader("data/orders.csv");
  // std::vector<Data> inputData = loader.loadData();

  // std::cout << "Timestamp\t\tOrder
  // ID\t\tInstrument\tSide\tType\tQuantity\tPrice\tAction" << std::endl;

  // for(Data order : inputData)
  // {
  //     std::cout << order.timestamp << "\t" << order.order_id << "\t" <<
  //     order.instrument << "\t"; std::cout << (order.side == Side::BUY ? "BUY"
  //     : "SELL") << "\t"; std::cout << (order.type == OrderType::LIMIT ?
  //     "LIMIT" : "MARKET") << "\t"; std::cout << order.quantity << "\t\t" <<
  //     order.price << "\t";

  //     if (order.action == Action::NEW) std::cout << "NEW";
  //     else if (order.action == Action::MODIFY) std::cout << "MODIFY";
  //     else if (order.action == Action::CANCEL) std::cout << "CANCEL";

  //     std::cout << std::endl;
  // }

  // // 2. Convertir vers le format de sortie
  // DataConverter converter;
  // std::vector<OutputData> outputData = converter.convertToOutput(inputData);

  // // 3. Moteur de trading

  // // 4. Écrire le fichier de sortie
  // CSVWriter writer("output_results.csv");
  // writer.writeHeader();
  // writer.writeData(outputData);
  // writer.close();
  // std::cout << "Output OKKKKKKKKKK" << std::endl;
  }