#include "data/DataLoader.h"
#include "data/CSVReader.h"

#include <iostream>

DataLoader::DataLoader(const std::string filename) : filename(filename)
{
    
}

std::vector<Data> DataLoader::loadData()
{
    std::vector<Data> data;
    CSVReader reader(filename);
    std::vector<CSVRow> rows = reader.readCSV();
    for (CSVRow row : rows)
    {
        Data d;
        d.timestamp = std::stoi(row.values[0]);
        d.open = std::stod(row.values[1]);
        d.high = std::stod(row.values[2]);
        d.low = std::stod(row.values[3]);
        d.close = std::stod(row.values[4]);
        d.volume = std::stod(row.values[5]);
        data.push_back(d);
    }
    return data;
}