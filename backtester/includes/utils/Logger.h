#ifndef LOGGER_H
#define LOGGER_H

#include "../core/Order.h"
#include <iostream>
#include <sstream>

class Logger {
public:
  Logger();
  Logger(std::ostream *output_stream);
  void writeHeader();

  void logOrderExecution(const Order &order, int executed_quantity,
                         double execution_price, long long counterparty_id,
                         long long timestamp);
  void logOrderPartialExecution(const Order &order, int executed_quantity,
                                double execution_price,
                                long long counterparty_id, long long timestamp);
  void logOrderPending(const Order &order, long long timestamp);
  void logOrderCancellation(const Order &order, long long timestamp);
  void logOrderModification(const Order &order, long long timestamp);
  void logOrderRejection(const Order &order, long long timestamp);

  void setOutputStream(std::ostream *output_stream);

private:
  std::ostream *output_stream;
  bool header_written;
};

#endif // LOGGER_H