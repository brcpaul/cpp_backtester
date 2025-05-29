#include <iostream>
#include "data/DataLoader.h"
#include "data/CSVWriter.h"
#include "data/DataConverter.h"

int main()
{
    // 1. Charger les données d'entrée
    DataLoader loader("data/orders.csv");
    std::vector<Data> inputData = loader.loadData();

    std::cout << "Timestamp\t\tOrder ID\t\tInstrument\tSide\tType\tQuantity\tPrice\tAction" << std::endl;

    for(Data order : inputData)
    {
        std::cout << order.timestamp << "\t" << order.order_id << "\t" << order.instrument << "\t";
        std::cout << (order.side == Side::BUY ? "BUY" : "SELL") << "\t";
        std::cout << (order.type == OrderType::LIMIT ? "LIMIT" : "MARKET") << "\t";
        std::cout << order.quantity << "\t\t" << order.price << "\t";

        if (order.action == Action::NEW) std::cout << "NEW";
        else if (order.action == Action::MODIFY) std::cout << "MODIFY";
        else if (order.action == Action::CANCEL) std::cout << "CANCEL";
        
        std::cout << std::endl;    
    }

    // 2. Convertir vers le format de sortie
    DataConverter converter;
    std::vector<OutputData> outputData = converter.convertToOutput(inputData);

    // 3. Moteur de trading

    // 4. Écrire le fichier de sortie
    CSVWriter writer("output_results.csv");
    writer.writeHeader();
    writer.writeData(outputData);
    writer.close();
    std::cout << "Output OKKKKKKKKKK" << std::endl;
}