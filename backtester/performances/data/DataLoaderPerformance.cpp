#include <iostream>
#include <fstream>
#include <chrono>
#include "../../includes/data/DataLoader.h"

// Génère un fichier CSV valide pour tester DataLoader
void generateValidOrdersCSV(const std::string& filename, size_t rowCount) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible de créer le fichier : " + filename);
    }

    // En-tête
    file << "timestamp,order_id,instrument,side,type,quantity,price,action\n";

    for (size_t i = 0; i < rowCount; ++i) {
        long long timestamp = 1650000000000000000 + i; // timestamp unique à 19 chiffres
        long long order_id = 1000 + i;
        std::string instrument = "AAPL";
        std::string side = (i % 2 == 0) ? "BUY" : "SELL";
        std::string type = (i % 3 == 0) ? "LIMIT" : "MARKET";
        long long quantity = 10 + (i % 50);
        std::string price = (type == "LIMIT") ? "150.25" : "";
        std::string action = "NEW";

        file << timestamp << "," << order_id << "," << instrument << "," << side << "," 
             << type << "," << quantity << "," << price << "," << action << "\n";
    }
}

// Test de performance pour DataLoader
void testDataLoaderPerformance(const std::string& filename, size_t expectedRows) {
    DataLoader loader(filename);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Data> data = loader.loadData();
    auto end = std::chrono::high_resolution_clock::now();

    double timeMs = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Fichier : " << filename << "\n"
              << "Ordres chargés : " << data.size() << "\n"
              << "Attendus : " << expectedRows << "\n"
              << "Temps : " << timeMs << " ms\n"
              << ((data.size() == expectedRows) ? "[SUCCÈS]" : "[ÉCHEC]") << "\n\n";
}

void runDataLoaderPerf() {
    std::cout << "\n=== Tests de performance DataLoader ===\n\n";

    try {
        const std::vector<size_t> sizes = {1000, 10000, 50000, 100000};

        for (size_t size : sizes) {
            std::string filename = "orders_" + std::to_string(size) + ".csv";
            generateValidOrdersCSV(filename, size);
            testDataLoaderPerformance(filename, size);

            // Suppression du fichier après test
            if (std::remove(filename.c_str()) != 0) {
                std::cerr << "Erreur lors de la suppression du fichier " << filename << "\n";
            } else {
                std::cout << "Fichier supprimé : " << filename << "\n";
            }
        }

        std::cout << "Tous les tests DataLoader sont terminés.\n";
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
    }

    std::cout << "=== Fin des tests DataLoader ===\n";
}

