#include "../../includes/utils/Logger.h"

Logger::Logger() : output_stream(&std::cout) {}

Logger::Logger(std::ostream *output_stream) : output_stream(output_stream) {}

void Logger::setOutputStream(std::ostream *output_stream) {
  this->output_stream = output_stream;
}

void Logger::logNewOrder(const Order &order, 
                          long long timestamp) {
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.quantity << "," 
      << order.price << "," << order.status << ",0,0,0";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderExecution(const Order &order, 
                                long long timestamp) {
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.quantity << ","
      << order.price << ",EXECUTED," << order.executed_quantity << ","
      << order.execution_price << "," << order.execution_price;
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderPartialExecution(const Order &order, 
                                      int executed_quantity,
                                      double execution_price,
                                      long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",PARTIALLY_EXECUTED," << executed_quantity << ","
      << execution_price << "," << order.execution_price;
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderPending(const Order &order, long long timestamp) {
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",PENDING," << order.executed_quantity << ","
      << order.execution_price << "," << order.execution_price;
  *output_stream << oss.str() << std::endl;
}