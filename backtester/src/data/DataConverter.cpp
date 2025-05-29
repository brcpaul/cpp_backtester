#include "data/DataConverter.h"

DataConverter::DataConverter() {
    // Constructeur par défaut
}

OutputData DataConverter::convertToOutput(const Data& inputData) {
    OutputData output;
    
    // Copie des données originales
    output.timestamp = inputData.timestamp;
    output.order_id = inputData.order_id;
    output.instrument = inputData.instrument;
    output.side = sideToString(inputData.side);
    output.type = orderTypeToString(inputData.type);
    output.quantity = inputData.quantity;
    output.price = inputData.price;
    output.action = actionToString(inputData.action);
    
    // Initialisation des nouvelles colonnes avec des valeurs par défaut
    // Ces valeurs seront mises à jour par le moteur de trading
    output.status = OrderStatus::PENDING;
    output.executed_quantity = 0.0;
    output.execution_price = 0.0;
    output.counterparty_id = "";
    
    return output;
}

std::vector<OutputData> DataConverter::convertToOutput(const std::vector<Data>& inputDataVector) {
    std::vector<OutputData> outputVector;
    outputVector.reserve(inputDataVector.size());
    
    for (const auto& inputData : inputDataVector) {
        outputVector.push_back(convertToOutput(inputData));
    }
    
    return outputVector;
}

std::string DataConverter::sideToString(Side side) {
    switch (side) {
        case Side::BUY:
            return "BUY";
        case Side::SELL:
            return "SELL";
        default:
            return "UNKNOWN";
    }
}

std::string DataConverter::orderTypeToString(OrderType type) {
    switch (type) {
        case OrderType::LIMIT:
            return "LIMIT";
        case OrderType::MARKET:
            return "MARKET";
        default:
            return "UNKNOWN";
    }
}

std::string DataConverter::actionToString(Action action) {
    switch (action) {
        case Action::NEW:
            return "NEW";
        case Action::MODIFY:
            return "MODIFY";
        case Action::CANCEL:
            return "CANCEL";
        default:
            return "UNKNOWN";
    }
}