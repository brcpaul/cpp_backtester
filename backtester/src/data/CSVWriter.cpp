#include "data/CSVWriter.h"
#include <iostream>
#include <iomanip>
#include <sstream>

CSVWriter::CSVWriter(const std::string& filename) : filename(filename) {
    file.open(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
}

CSVWriter::~CSVWriter() {
    file.close();
}

void CSVWriter::writeHeader() {
    file << "timestamp,order_id,instrument,side,type,quantity,price,action,"
         << "status,executed_quantity,execution_price,counterparty_id\n" << std::endl;
}

void CSVWriter::writeData(const OutputData& data) {
    file << data.timestamp << ","
         << data.order_id << ","
         << data.instrument << ","
         << data.side << ","
         << data.type << ","
         << std::fixed << std::setprecision(6) << data.quantity << ","
         << std::fixed << std::setprecision(6) << data.price << ","
         << data.action << ","
         << orderStatusToString(data.status) << ","
         << std::fixed << std::setprecision(6) << data.executed_quantity << ","
         << std::fixed << std::setprecision(6) << data.execution_price << ","
         << data.counterparty_id << std::endl;
}

void CSVWriter::writeData(const std::vector<OutputData>& dataVector) {
    for (const auto& data : dataVector) {
        writeData(data);
    }
}

void CSVWriter::close() {
    file.close();
}

std::string CSVWriter::orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::EXECUTED: return "EXECUTED";
        case OrderStatus::PARTIALLY_EXECUTED: return "PARTIALLY_EXECUTED";
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::CANCELED: return "CANCELED";
        case OrderStatus::REJECTED: return "REJECTED";
        default: return "UNKNOWN";
    }
}