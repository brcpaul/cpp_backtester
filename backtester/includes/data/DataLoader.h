#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include "../core/Order.h"

enum class Side {
    BUY,
    SELL
};

enum class Action {
    NEW,
    MODIFY,
    CANCEL
};

struct Data
{
    long long timestamp;        // Horodatage en nanosecondes depuis l'époque Unix
    std::string order_id;       // Identifiant unique de l'ordre
    std::string instrument;     // Code de l'instrument financier
    Side side;                  // Côté de l'ordre (BUY ou SELL)
    OrderType type;             // Type d'ordre (LIMIT, MARKET)
    double quantity;            // Quantité de l'instrument
    double price;               // Prix limite (pour les ordres LIMIT)
    Action action;              // Action à effectuer (NEW, MODIFY, CANCEL)
};

class DataLoader
{
public:
    DataLoader(const std::string filename);
    std::vector<Data> loadData();

private:
    std::string filename;
    std::string trim(const std::string& str);  
    Side parseSide(const std::string& sideStr);
    OrderType parseOrderType(const std::string& typeStr);
    Action parseAction(const std::string& actionStr);
};

#endif // DATALOADER_H