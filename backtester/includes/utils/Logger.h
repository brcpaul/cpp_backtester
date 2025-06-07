#ifndef LOGGER_H
#define LOGGER_H

#include "../core/Order.h"
#include <iostream>
#include <sstream>

// Mettre dans utils peut être ? 
// (const std::string& filename) : filename(filename) {
//     file.open(filename, std::ios::out | std::ios::trunc);
//     if (!file.is_open()) {
//         throw std::runtime_error("Cannot open file for writing: " + filename);
//     }
// }

class Logger {
public:
  Logger();
  Logger(std::ostream *output_stream);

  void logNewOrder(const Order &order, long long timestamp);
  void logOrderExecution(const Order &order, long long timestamp);
  void logOrderPartialExecution(const Order &order, int executed_quantity,
                                double execution_price, long long timestamp);
  void logOrderPending(const Order &order, long long timestamp);
  void logOrderCancellation(const Order &order, long long timestamp);
  void logOrderModification(const Order &order, long long timestamp);
  void logOrderRejection(const Order &order, long long timestamp);

  void setOutputStream(std::ostream *output_stream);

private:
  std::ostream *output_stream;
};

#endif // LOGGER_H