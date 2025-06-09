#include "../../includes/data/OrderGenerator.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <chrono>

OrderGenerator::OrderGenerator()
    : instruments{"AAPL", "EURUSD", "BTCUSD", "TSLA"}, sides{"BUY", "SELL"},
      types{"LIMIT", "MARKET"}, actions{"NEW", "NEW", "NEW", "MODIFY",
                                        "CANCEL"},
      gen(rd()), quantity_dist(1, 1000), price_dist(-0.05, 0.05),
      instrument_dist(0, instruments.size() - 1),
      side_dist(0, sides.size() - 1), type_dist(0, types.size() - 1),
      action_dist(0, actions.size() - 1), last_order_id(0) {
  last_instrument_price["AAPL"] = 150.0;
  last_instrument_price["EURUSD"] = 1.1;
  last_instrument_price["BTCUSD"] = 60000.0;
  last_instrument_price["TSLA"] = 700.0;
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
  order.instrument = instruments[instrument_dist(gen)];
  order.side = sides[side_dist(gen)];
  order.type = types[type_dist(gen)];
  order.quantity = quantity_dist(gen);
  order.price =
      (order.type == "LIMIT")
          ? round_to_2_decimals(last_instrument_price[order.instrument] +
                                price_dist(gen))
          : 0.0;
  if (order.side == "BUY") {
    order.price -= 0.06;
  } else {
    order.price += 0.06;
  }
  order.action = actions[action_dist(gen)];
  if (order.action == "NEW") {
    order.order_id = last_order_id++;
  } else {
    order.order_id = order_id_dist(gen);
  }

  return order;
}

void OrderGenerator::export_to_csv(const std::vector<RandomOrder> &orders,
                                   const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Erreur : Impossible d'ouvrir le fichier : " +
                             filename);
  }

  // Écriture de l'en-tête
  file << "timestamp,order_id,instrument,side,type,quantity,price,action\n";

  // Écriture des lignes de données
  for (const auto &order : orders) {
    file << order.timestamp << ',' << order.order_id << ',' << order.instrument
         << ',' << order.side << ',' << order.type << ',' << order.quantity
         << ',' << std::fixed << std::setprecision(2) << order.price << ','
         << order.action << '\n';
  }

  std::cout << "Fichier CSV généré avec succès : " << filename << std::endl;
}
