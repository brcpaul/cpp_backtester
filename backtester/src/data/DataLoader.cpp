#include "data/DataLoader.h"
#include "data/CSVReader.h"

#include <iostream>
#include <algorithm> 
#include <cctype> 

DataLoader::DataLoader(const std::string filename) : filename(filename)
{
    
}

// Fonction utilitaire pour nettoyer les chaînes
std::string DataLoader::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::vector<Data> DataLoader::loadData()
{
    std::vector<Data> data;
    CSVReader reader(filename);
    std::vector<CSVRow> rows = reader.readCSV();

    bool firstRow = true;

    for (CSVRow row : rows)
    {
        // Skip the header row
        if (firstRow)
        {
            firstRow = false;
            continue;
        }

        Data order;
        order.timestamp = std::stoll(trim(row.values[0]));
        order.order_id = trim(row.values[1]);
        order.instrument = trim(row.values[2]);
        order.side = parseSide(trim(row.values[3]));
        order.type = parseOrderType(trim(row.values[4]));
        order.quantity = std::stod(trim(row.values[5]));
        
        // Pas de prix pour les MARKET orders
        std::string priceStr = trim(row.values[6]);
        if (priceStr.empty()) {
            order.price = 0.0; 
        } else {
            order.price = std::stod(priceStr);
        }
        
        order.action = parseAction(trim(row.values[7]));
        
        data.push_back(order);
    }
    return data;
}

Side DataLoader::parseSide(const std::string& sideStr)
{
    std::string upperSide = sideStr;
    std::transform(upperSide.begin(), upperSide.end(), upperSide.begin(), ::toupper);
    
    if (upperSide == "BUY")
        return Side::BUY;
    else if (upperSide == "SELL")
        return Side::SELL;
    else
        throw std::invalid_argument("Invalid side: " + sideStr);
}

OrderType DataLoader::parseOrderType(const std::string& typeStr)
{
    std::string upperType = typeStr;
    std::transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);
    
    if (upperType == "LIMIT")
        return OrderType::LIMIT;
    else if (upperType == "MARKET")
        return OrderType::MARKET;
    else
        throw std::invalid_argument("Invalid order type: " + typeStr);
}

Action DataLoader::parseAction(const std::string& actionStr)
{
    std::string upperAction = actionStr;
    std::transform(upperAction.begin(), upperAction.end(), upperAction.begin(), ::toupper);
    
    if (upperAction == "NEW")
        return Action::NEW;
    else if (upperAction == "MODIFY")
        return Action::MODIFY;
    else if (upperAction == "CANCEL")
        return Action::CANCEL;
    else
        throw std::invalid_argument("Invalid action: " + actionStr);
}