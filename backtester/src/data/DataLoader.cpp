#include "data/DataLoader.h"
#include "data/CSVReader.h"

#include <iostream>
#include <algorithm> 
#include <cctype> 
#include <sstream>
#include <unordered_set>

// Constructeur
DataLoader::DataLoader(const std::string filename) : filename(filename) {}

// Fonction utilitaire pour nettoyer les chaînes
std::string DataLoader::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Verification des entrées 
bool DataLoader::isValidInt(const std::string& inputStr) {   // Vérifie si order_id/quantity est un nombre entier positif
    return !inputStr.empty() && std::all_of(inputStr.begin(), inputStr.end(), ::isdigit);
}

bool DataLoader::isValidDouble(const std::string& inputStr) {   
    // Vérifie si price est un nombre positif
    if (inputStr.empty())
        return false;

    char* endptr = nullptr;
    const char* str = inputStr.c_str();
    double val = std::strtod(str, &endptr);

    // std::strtod renvoie un pointeur endptr à la position où il a arrêté la conversion
    // Si endptr pointe à la fin de la chaîne, c'est un double valide
    return (*endptr == '\0') && (val >= 0.0);
}

double DataLoader::parsePositiveDouble(const std::string& str, const std::string& fieldName) {
    std::string trimmed = trim(str);
    if (!isValidDouble(trimmed)) {
        throw std::invalid_argument("Invalid '" + fieldName + "' format: '" + trimmed + "' must be a positive number");
    }
    return std::stod(trimmed);
}

long long DataLoader::parsePositiveInt(const std::string& str, const std::string& fieldName) {
    std::string trimmed = trim(str);
    if (!isValidInt(trimmed)) {
        throw std::invalid_argument("Invalid '" + fieldName + "' format: '" + trimmed + "' must contain only digits");
    }
    return std::stoll(trimmed);
}

long long DataLoader::parseTimestamp(const std::string& str, const std::string& fieldName) {
    std::string trimmed = trim(str);
    if (!isValidInt(trimmed)) {
        throw std::invalid_argument("Invalid '" + fieldName + "' format: '" + trimmed + "' must contain only digits");
    }

    // Vérifie que le timestamp a exactement 19 chiffres (nanosecondes)
    if (trimmed.length() != 19) {
        throw std::invalid_argument("Invalid '" + fieldName + "' length: '"  + trimmed + "' must have exactly 19 digits (nanoseconds)");
    }

    return std::stoll(trimmed);
}

std::vector<Data> DataLoader::loadData() {
    std::vector<Data> data;
    CSVReader reader(filename);
    std::vector<CSVRow> rows = reader.readCSV();

    bool firstRow = true;

    std::unordered_set<long long> newOrderIds; // Pour suivre les order_id avec action NEW

    for (CSVRow row : rows)
    {
        // Skip the header row
        if (firstRow)
        {
            firstRow = false;
            continue;
        }

        Data order;
        order.timestamp =  parseTimestamp(row.values[0], "timestamp");
        order.order_id = parsePositiveInt(row.values[1], "order_id");
        order.instrument = trim(row.values[2]);
        order.side = parseSide(trim(row.values[3]));
        order.type = parseOrderType(trim(row.values[4]));
        order.quantity = parsePositiveInt(row.values[5], "quantity");
        
        order.action = parseAction(trim(row.values[7]));
        // Vérification des doublons pour les ordres NEW
        if (order.action == Action::NEW) {
            if (newOrderIds.find(order.order_id) != newOrderIds.end()) {
                throw std::invalid_argument("Duplicate order_id " + std::to_string(order.order_id) + " with action NEW found\nPlease ensure that each NEW order has a unique order_id.");
            }
            newOrderIds.insert(order.order_id);
        }
        
        // Gestion du prix selon le type d'ordre
        std::string priceStr = trim(row.values[6]);
        if (order.action == Action::CANCEL) {
            // Pour CANCEL, le prix n'est pas requis (peu importe le type)
            order.price = 0.0;
        } else if (order.type == OrderType::MARKET) {
            // Pour les ordres MARKET, le prix peut être vide (on met 0.0 par défaut)
            order.price = 0.0;
        } else if (order.type == OrderType::LIMIT) {
            // Pour les ordres LIMIT, le prix est obligatoire
            if (priceStr.empty()) {
                throw std::invalid_argument("Price is required for LIMIT orders but was empty for order_id: " + std::to_string(order.order_id));
            }
            order.price = parsePositiveDouble(priceStr, "price");
        }

        
        
        data.push_back(order);
    }
    return data;
}

Side DataLoader::parseSide(const std::string& sideStr) {   
    std::string upperSide = sideStr;
    std::transform(upperSide.begin(), upperSide.end(), upperSide.begin(), ::toupper);
    
    if (upperSide == "BUY") return Side::BUY;
    if (upperSide == "SELL") return Side::SELL;
    throw std::invalid_argument("Error in column 'side', error value: " + sideStr);
}

OrderType DataLoader::parseOrderType(const std::string& typeStr) {
    std::string upperType = typeStr;
    std::transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);
    
    if (upperType == "LIMIT") return OrderType::LIMIT;
    if (upperType == "MARKET") return OrderType::MARKET;
    throw std::invalid_argument("Error in column 'order type',  error value: " + typeStr);
}

Action DataLoader::parseAction(const std::string& actionStr) {
    std::string upperAction = actionStr;
    std::transform(upperAction.begin(), upperAction.end(), upperAction.begin(), ::toupper);
    
    if (upperAction == "NEW") return Action::NEW;
    if (upperAction == "MODIFY") return Action::MODIFY;
    if (upperAction == "CANCEL") return Action::CANCEL;
    throw std::invalid_argument("Error in column 'action',  error value: " + actionStr);
}