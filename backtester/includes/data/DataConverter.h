#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include "DataLoader.h"
#include "CSVWriter.h"
#include <vector>

class DataConverter {
public:
    DataConverter();
    
    // Convertit les données d'entrée vers le format de sortie
    OutputData convertToOutput(const Data& inputData);
    std::vector<OutputData> convertToOutput(const std::vector<Data>& inputDataVector);
    
    // Méthodes utilitaires pour la conversion des enums vers strings
    static std::string sideToString(Side side);
    static std::string orderTypeToString(OrderType type);
    static std::string actionToString(Action action);
};

#endif