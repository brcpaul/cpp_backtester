#include "../../includes/utils/Logger.h"

Logger::Logger() : output_stream(&std::cout) {}

Logger::Logger(std::ostream *output_stream) : output_stream(output_stream) {}

void Logger::setOutputStream(std::ostream *output_stream) {
  this->output_stream = output_stream;
  writeHeader();
}

void Logger::writeHeader() {
  *output_stream << "timestamp,order_id,instrument,side,type,quantity,price,"
                    "status,executed_quantity,execution_price,counterparty_id"
                 << std::endl;
}

void Logger::logOrderExecution(const Order &order, int executed_quantity,
                               double execution_price,
                               long long counterparty_id, long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",EXECUTED," << executed_quantity << ","
      << execution_price << "," << counterparty_id;
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderPartialExecution(const Order &order, int executed_quantity,
                                      double execution_price,
                                      long long counterparty_id,
                                      long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",PARTIALLY_EXECUTED," << executed_quantity << ","
      << execution_price << "," << counterparty_id;
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderPending(const Order &order, long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",PENDING," << order.executed_quantity << ","
      << order.execution_price << ",0";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderCancellation(const Order &order, long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",CANCELED,0,0,0";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderRejection(const Order &order, long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," <<remaining_quantity << ","
      << order.price << ",REJECTED,0,0,0";
  *output_stream << oss.str() << std::endl;
}