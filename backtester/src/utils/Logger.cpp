#include "../../includes/utils/Logger.h"

Logger::Logger() : output_stream(&std::cout), header_written(false) {}

Logger::Logger(std::ostream *output_stream) : output_stream(output_stream), header_written(false) {}

void Logger::setOutputStream(std::ostream *output_stream) {
  this->output_stream = output_stream;
  this->header_written = false;
}

void Logger::writeHeader() {
  if (!header_written && output_stream) {
    *output_stream << "timestamp,order_id,instrument,side,type,quantity,price,status,executed_quantity,execution_price,counterparty_id" << std::endl;
    header_written = true;
  }
}

void Logger::logNewOrder(const Order &order, 
                          long long timestamp) {
  writeHeader();
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.quantity << "," 
      << order.price << "," << order.status << ",0,0,0";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderExecution(const Order &order, 
                                long long timestamp,
                                long long counterparty_order_id) {
  writeHeader();
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",EXECUTED," << order.executed_quantity << ","
      << order.execution_price << "," << counterparty_order_id;
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderPartialExecution(const Order &order, 
                                      int executed_quantity,
                                      double execution_price,
                                      long long timestamp,
                                      long long counterparty_order_id) {
  writeHeader();
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",PARTIALLY_EXECUTED," << executed_quantity << ","
      << execution_price << "," << counterparty_order_id;
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderPending(const Order &order, 
                              long long timestamp) {
  writeHeader();
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",PENDING," << ",0,0,0";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderModification(const Order &order, 
                                   long long timestamp) {
  writeHeader();
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",MODIFIED";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderCancellation(const Order &order, 
                                   long long timestamp) {
  writeHeader();
  std::ostringstream oss;
  int remaining_quantity = order.quantity - order.executed_quantity;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << remaining_quantity << ","
      << order.price << ",CANCELED";
  *output_stream << oss.str() << std::endl;
}

void Logger::logOrderRejection(const Order &order, 
                                long long timestamp,
                                const std::string &reason) {
  writeHeader();
  std::ostringstream oss;
  oss << timestamp << "," << order.order_id << "," << order.instrument << ","
      << order.side << "," << order.type << "," << order.quantity << ","
      << order.price << ",REJECTED," << reason;
  *output_stream << oss.str() << std::endl;
}