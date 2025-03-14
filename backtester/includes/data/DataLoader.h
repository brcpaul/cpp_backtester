#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>

struct Data
{
    int timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class DataLoader
{
public:
    DataLoader(const std::string filename);
    std::vector<Data> loadData();

private:
    std::string filename;
};

#endif // DATALOADER_H