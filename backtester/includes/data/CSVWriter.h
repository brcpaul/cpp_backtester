#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <string>
#include <vector>
#include <fstream>

enum class OrderStatus {
    EXECUTED,
    PARTIALLY_EXECUTED,
    PENDING,
    CANCELED,
    REJECTED
};

struct OutputData {
    // Données originales
    long long timestamp;
    std::string order_id;
    std::string instrument;
    std::string side;
    std::string type;
    double quantity;
    double price;
    std::string action;
    
    // Nouvelles colonnes pour l'output
    OrderStatus status;
    double executed_quantity;
    double execution_price;
    std::string counterparty_id;
    
    // Constructeurs
    OutputData() : timestamp(0), quantity(0.0), price(0.0), 
                   status(OrderStatus::PENDING), executed_quantity(0.0), 
                   execution_price(0.0) {}
};

class CSVWriter {
public:
    CSVWriter(const std::string& filename);
    ~CSVWriter();
    
    void writeHeader();
    void writeData(const OutputData& data);
    void writeData(const std::vector<OutputData>& dataVector);
    void close();

private:
    std::ofstream file;
    std::string filename;
    
    std::string orderStatusToString(OrderStatus status);
    std::string escapeCSV(const std::string& field);
};

#endif 