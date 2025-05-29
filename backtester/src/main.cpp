#include <iostream>
#include "data/DataLoader.h"

int main()
{

    DataLoader loader("data/orders.csv");
    std::vector<Data> data = loader.loadData();

    std::cout << "Timestamp\t\tOrder ID\t\tInstrument\tSide\tType\tQuantity\tPrice\tAction" << std::endl;

    for(Data order : data)
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

}