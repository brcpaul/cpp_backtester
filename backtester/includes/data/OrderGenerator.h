#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <vector>
#include <string>
#include <random>
#include <unordered_map>

struct RandomOrder {
    long long timestamp;
    int order_id;
    std::string instrument;
    std::string side;
    std::string type;
    int quantity;
    double price;
    std::string action;
};

class OrderGenerator {
private:

    friend void testRounding();
    
    std::vector<std::string> instruments;
    std::vector<std::string> sides;
    std::vector<std::string> types;
    std::vector<std::string> actions;
    
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> quantity_dist;
    std::uniform_real_distribution<double> price_dist;
    std::uniform_int_distribution<size_t> instrument_dist;
    std::uniform_int_distribution<size_t> side_dist;
    std::uniform_int_distribution<size_t> type_dist;
    std::uniform_int_distribution<size_t> action_dist;

    std::unordered_map<std::string, double> last_instrument_price;
    int last_order_id;

    // Générateur d'horodatage en nanosecondes
    long long current_timestamp_ns();
    // Fonction privée pour arrondir à 2 décimales
    double round_to_2_decimals(double value);

public:
    OrderGenerator();
    
    // Génération d'un ordre fictif
    RandomOrder generate_order();
    
    // Export vers CSV
    void export_to_csv(const std::vector<RandomOrder>& orders, const std::string& filename);
};

#endif 