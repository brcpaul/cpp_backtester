#include "../../includes/data/OrderGenerator.h"
#include <iostream>
#include <chrono>

void testOrderGenerationPerformance(size_t numOrders) {
    OrderGenerator generator;

    std::cout << "Génération de " << numOrders << " ordres...\n";

    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<RandomOrder> orders;
    orders.reserve(numOrders);
    for (size_t i = 0; i < numOrders; ++i) {
        orders.push_back(generator.generate_order());
    }

    auto end = std::chrono::high_resolution_clock::now();
    double generationTime = std::chrono::duration<double, std::milli>(end - start).count();

    std::string filename = "generated_orders_" + std::to_string(numOrders) + ".csv";

    auto exportStart = std::chrono::high_resolution_clock::now();
    generator.export_to_csv(orders, filename);
    auto exportEnd = std::chrono::high_resolution_clock::now();
    double exportTime = std::chrono::duration<double, std::milli>(exportEnd - exportStart).count();

    std::cout << "Temps de génération : " << generationTime << " ms\n";
    std::cout << "Temps d'export CSV  : " << exportTime << " ms\n";
    std::cout << "[Terminé] -> " << filename << "\n\n";

    // Suppression du fichier après test
    if (std::remove(filename.c_str()) != 0) {
        std::cerr << "Erreur lors de la suppression du fichier " << filename << "\n";
    } else {
        std::cout << "Fichier supprimé : " << filename << "\n\n";
    }
}

void runOrderGeneratorPerf() {
    std::cout << "=== Tests de performance OrderGenerator ===\n\n";

    try {
        std::vector<size_t> sizes = {1000, 10000, 50000, 100000};

        for (size_t size : sizes) {
            testOrderGenerationPerformance(size);
        }

        std::cout << "=== Fin des tests OrderGenerator ===\n";
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
    }
}
