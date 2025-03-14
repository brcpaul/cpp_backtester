#include <iostream>
#include "data/DataLoader.h"

int main()
{

    DataLoader loader("../data/btcusdt.csv");
    std::vector<Data> data = loader.loadData();

    std::cout << "Timestamp\tOpen\tHigh\tLow\tClose\tVolume" << std::endl;

    for(Data d : data)
    {
        std::cout << d.timestamp << "\t" << d.open << "\t" << d.high << "\t" << d.low << "\t" << d.close << "\t" << d.volume << std::endl;
    }

}