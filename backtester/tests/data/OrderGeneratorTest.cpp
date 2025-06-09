#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <set>
#include <map>
#include "../includes/data/OrderGenerator.h"
#include "../utils/TestFile.h"

void testOrderGeneration() {
    std::cout << "Test: Génération d'ordres... ";
    
    OrderGenerator generator;
    std::map<std::string, int> instrumentCount, sideCount, typeCount;
    
    for (int i = 0; i < 100; ++i) {
        RandomOrder order = generator.generate_order();
        
        // Vérifications basiques
        assert(order.timestamp > 0);
        assert(order.order_id >= 1 && order.order_id <= 1000);
        assert(order.quantity >= 1 && order.quantity <= 1000);
        
        // Logique des prix
        if (order.type == "LIMIT") {
            assert(order.price > 0.0 && order.price <= 1000.0);
        } else {
            assert(order.price == 0.0);
        }
        
        // Comptage pour distribution
        instrumentCount[order.instrument]++;
        sideCount[order.side]++;
        typeCount[order.type]++;
    }
    
    // Vérification distribution
    assert(instrumentCount.size() == 4); // AAPL, EURUSD, BTCUSD, TSLA
    assert(sideCount.size() == 2);       // BUY, SELL
    assert(typeCount.size() == 2);       // LIMIT, MARKET
    
    std::cout << "PASS\n";
}

void testRounding() {
    std::cout << "Test: Arrondi... ";
    
    OrderGenerator generator;
    assert(std::abs(generator.round_to_2_decimals(123.456) - 123.46) < 0.001);
    assert(std::abs(generator.round_to_2_decimals(123.0) - 123.0) < 0.001);
    
    std::cout << "PASS\n";
}

void testCSVExport() {
    std::cout << "Test: Export CSV... ";
    
    OrderGenerator generator;
    std::vector<RandomOrder> orders;
    
    // Génère 3 ordres
    for (int i = 0; i < 3; ++i) {
        orders.push_back(generator.generate_order());
    }
    
    std::string filename = "test_orders.csv";
    generator.export_to_csv(orders, filename);
    
    // Vérification fichier
    assert(std::filesystem::exists(filename));
    
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    assert(line == "timestamp,order_id,instrument,side,type,quantity,price,action");
    
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
    }
    assert(lineCount == 3);
    
    file.close();
    cleanupFile(filename);
    
    // Test erreur fichier invalide
    bool exceptionThrown = false;
    try {
        generator.export_to_csv(orders, "/invalid/path.csv");
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    
    std::cout << "PASS\n";
}

void runOrderGeneratorTests() {
    std::cout << "=== Tests OrderGenerator ===\n\n";

    try { testOrderGeneration(); } catch (...) { std::cout << "FAIL\n"; }
    try { testRounding(); } catch (...) { std::cout << "FAIL\n"; }
    try { testCSVExport(); } catch (...) { std::cout << "FAIL\n"; }

    std::cout << "\n=== Fin des tests OrderGenerator ===\n";
}