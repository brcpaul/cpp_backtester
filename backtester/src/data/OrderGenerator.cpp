#include "../../includes/data/OrderGenerator.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

OrderGenerator::OrderGenerator()
    : instruments({"AAPL"}), sides({"BUY", "SELL"}), types({"LIMIT", "MARKET"}),
      actions({"NEW", "MODIFY", "CANCEL"}), gen(rd()), quantity_dist(1, 1000),
      price_dist(-0.1, 0.1) {
  last_instrument_price["AAPL"] = 150.0;
  //   last_instrument_price["EURUSD"] = 1.1;
  //   last_instrument_price["BTCUSD"] = 60000.0;
  //   last_instrument_price["TSLA"] = 700.0;
  last_order_id = 0;
}

long long OrderGenerator::current_timestamp_ns() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

double OrderGenerator::round_to_2_decimals(double value) {
  return std::round(value * 100.0) / 100.0;
}

RandomOrder OrderGenerator::generate_order() {
  RandomOrder order;
  order.timestamp = current_timestamp_ns();
  order.order_id = last_order_id++;

  // Use separate distributions for each vector to avoid conflicts
  std::uniform_int_distribution<size_t> instrument_dist(0,
                                                        instruments.size() - 1);
  std::uniform_int_distribution<size_t> side_dist(0, sides.size() - 1);
  std::uniform_int_distribution<size_t> type_dist(0, types.size() - 1);
  std::uniform_int_distribution<size_t> action_dist(0, actions.size() - 1);

  order.instrument = instruments[instrument_dist(gen)];
  order.side = sides[side_dist(gen)];
  order.type = types[type_dist(gen)];
  order.quantity = quantity_dist(gen);

  if (order.type == "LIMIT") {
    order.price = round_to_2_decimals(last_instrument_price[order.instrument] +
                                      price_dist(gen));
    if (order.side == "BUY") {
      order.price -= 0.06;
    } else {
      order.price += 0.06;
    }

    last_instrument_price[order.instrument] = order.price;
  } else {
    order.price = 0.0;
  }

  order.action = actions[action_dist(gen)];

  return order;
}

void OrderGenerator::export_to_csv(const std::vector<RandomOrder> &orders,
                                   const std::string &filename) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename
              << std::endl;
    return;
  }

  // En-têtes CSV
  file << "timestamp,order_id,instrument,side,type,quantity,price,action\n";

  // Données
  for (const auto &order : orders) {
    file << order.timestamp << "," << order.order_id << "," << order.instrument
         << "," << order.side << "," << order.type << "," << order.quantity
         << "," << std::fixed << std::setprecision(2) << order.price << ","
         << order.action << "\n";
  }

  file.close();
  std::cout << "Fichier " << filename << " généré avec succès." << std::endl;
}