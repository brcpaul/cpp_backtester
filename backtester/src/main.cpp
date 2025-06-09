#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"
#include "../includes/data/CSVReader.h"
#include "../includes/data/DataLoader.h"
#include "../includes/utils/Logger.h"
#include "../includes/data/OrderGenerator.h"

#include <iostream>
#include <vector>
#include <fstream>

int main() {
    // 1. Charger les données CSV / Créer des ordres aléatoires
    const bool random_orders = true;

    // Déclaration de loader 
    DataLoader loader("");  // Initialisation vide temporaire

    if (random_orders) {
        OrderGenerator generator;

        // Nombre d'ordres à générer
        const int num_orders = 100000;
        std::vector<RandomOrder> orders;

        // Génération des ordres
        for (int i = 0; i < num_orders; ++i) {
            orders.push_back(generator.generate_order());
        }

        // Export vers CSV
        generator.export_to_csv(orders, "data/random_orders.csv");

        // Recharge les données générées
        loader = DataLoader("data/random_orders.csv");
    } else {
        loader = DataLoader("data/orders.csv");
    }

    // Chargement des données
    std::vector<Data> inputData = loader.loadData();

    std::ofstream fileOutput("output.csv");

    MatchingEngine engine;
    engine.setOutputStream(&fileOutput);
    // engine.setOutputStream(&std::cout);  // alternative pour sortie console

    // 2. On traite chaque ligne du fichier CSV
    for (const auto &data : inputData) {
        OrderSide orderSide = (data.side == Side::BUY) ? OrderSide::BUY : OrderSide::SELL;

        // Création de l'ordre
        Order order(data.order_id, data.instrument, orderSide, data.type, data.price,
                    static_cast<int>(data.quantity), data.timestamp);

        // Actions selon le type
        if (data.action == Action::NEW) {
            engine.submitOrder(order);
        } else if (data.action == Action::MODIFY) {
            engine.modifyOrder(order);
        } else if (data.action == Action::CANCEL) {
            engine.cancelOrder(order);
        }
    }

    // 5. État final du carnet d’ordres pour debugger
    std::cout << engine.getOrderBook("AAPL").toString() << std::endl;

    return 0;
}
