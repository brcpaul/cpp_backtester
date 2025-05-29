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
    if (file.is_open()) {
        file.close();
    }
}

void CSVWriter::writeHeader() {
    file << "timestamp,order_id,instrument,side,type,quantity,price,action,"
         << "status,executed_quantity,execution_price,counterparty_id" << std::endl;
}

void CSVWriter::writeData(const OutputData& data) {
    file << data.timestamp << ","
         << escapeCSV(data.order_id) << ","
         << escapeCSV(data.instrument) << ","
         << escapeCSV(data.side) << ","
         << escapeCSV(data.type) << ","
         << std::fixed << std::setprecision(6) << data.quantity << ","
         << std::fixed << std::setprecision(6) << data.price << ","
         << escapeCSV(data.action) << ","
         << escapeCSV(orderStatusToString(data.status)) << ","
         << std::fixed << std::setprecision(6) << data.executed_quantity << ","
         << std::fixed << std::setprecision(6) << data.execution_price << ","
         << escapeCSV(data.counterparty_id) << std::endl;
}

void CSVWriter::writeData(const std::vector<OutputData>& dataVector) {
    for (const auto& data : dataVector) {
        writeData(data);
    }
}

void CSVWriter::close() {
    if (file.is_open()) {
        file.close();
    }
}

std::string CSVWriter::orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::EXECUTED:
            return "EXECUTED";
        case OrderStatus::PARTIALLY_EXECUTED:
            return "PARTIALLY_EXECUTED";
        case OrderStatus::PENDING:
            return "PENDING";
        case OrderStatus::CANCELED:
            return "CANCELED";
        case OrderStatus::REJECTED:
            return "REJECTED";
        default:
            return "UNKNOWN";
    }
}

std::string CSVWriter::escapeCSV(const std::string& field) {
    // Si le champ contient des virgules, guillemets ou retours à la ligne,
    // il faut l'entourer de guillemets et échapper les guillemets internes
    if (field.find(',') != std::string::npos || 
        field.find('"') != std::string::npos || 
        field.find('\n') != std::string::npos ||
        field.find('\r') != std::string::npos) {
        
        std::string escaped = "\"";
        for (char c : field) {
            if (c == '"') {
                escaped += "\"\"";  // Échapper les guillemets
            } else {
                escaped += c;
            }
        }
        escaped += "\"";
        return escaped;
    }
    return field;
}