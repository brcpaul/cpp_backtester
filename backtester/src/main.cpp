// #include "data/CSVWriter.h"
// #include "data/DataConverter.h"
// #include "data/DataLoader.h"

#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"

#include <iostream>

int main() {

  MatchingEngine engine;

  Order order;

  order = Order(1, "AAPL", OrderSide::BUY, OrderType::LIMIT, 100, 2,
                171717171717171717);

  engine.submitOrder(order);

  order = Order(2, "AAPL", OrderSide::SELL, OrderType::LIMIT, 102, 5,
                171717171717171717);

  engine.submitOrder(order);

  Order modifiedOrder = Order(2, "AAPL", OrderSide::SELL, OrderType::LIMIT, 103, 3,
                171717171717171717);

  engine.modifyOrder(modifiedOrder);

  order = Order(3, "AAPL", OrderSide::BUY, OrderType::LIMIT, 100, 2,
                171717171717171717);
  engine.submitOrder(order);

  order = Order(4, "AAPL", OrderSide::SELL, OrderType::MARKET, 103, 2,
                171717171717171717);
  engine.submitOrder(order);

  order = Order(5, "AAPL", OrderSide::SELL, OrderType::LIMIT, 99, 10,
                171717171717171717);
  engine.submitOrder(order);

  order = Order(6, "AAPL", OrderSide::BUY, OrderType::MARKET, 0, 10,
                171717171717171717);
  engine.submitOrder(order);

  std::cout << engine.getOrderBook("AAPL").toString() << std::endl;

  


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