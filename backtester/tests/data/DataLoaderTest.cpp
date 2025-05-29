#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <stdexcept>
#include "../includes/data/DataLoader.h"
#include "../utils/TestFile.h"

void testLoadValidData() {
    std::cout << "Test: Chargement de données valides... ";

    std::string file = "valid_test.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456,ORDER_001,AAPL,BUY,LIMIT,100.0,150.25,NEW\n"
        "1234567890123457,ORDER_002,GOOGL,SELL,MARKET,50.0,,NEW\n"
    );

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.size() == 2);
    assert(data[0].order_id == "ORDER_001");
    assert(data[0].side == Side::BUY);
    assert(data[1].price == 0.0); // prix vide -> 0.0

    cleanupFile(file);
    std::cout << "PASS\n";
}

void testInvalidSideThrows() {
    std::cout << "Test: Erreur sur side invalide... ";

    std::string file = "invalid_side.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456,ORDER_001,AAPL,BAD,LIMIT,100.0,150.25,NEW\n"
    );

    bool thrown = false;
    try {
        DataLoader loader(file);
        loader.loadData();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }

    assert(thrown);
    cleanupFile(file);
    std::cout << "PASS\n";
}

void testTrimmedValues() {
    std::cout << "Test: Espaces autour des champs... ";

    std::string file = "trim_test.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        " 123 , ORDER_1 , AAPL , buy , limit , 10 , 150.0 , new \n"
    );

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.size() == 1);
    assert(data[0].timestamp == 123);
    assert(data[0].order_id == "ORDER_1");
    assert(data[0].side == Side::BUY);
    assert(data[0].type == OrderType::LIMIT);
    assert(data[0].action == Action::NEW);

    cleanupFile(file);
    std::cout << "PASS\n";
}

void testEmptyFile() {
    std::cout << "Test: Fichier vide... ";

    std::string file = "empty.csv";
    createTestFile(file, "timestamp,order_id,instrument,side,type,quantity,price,action\n");

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.empty());

    cleanupFile(file);
    std::cout << "PASS\n";
}

void runDataLoaderTests() {
    std::cout << "=== Tests DataLoader ===\n\n";

    try { testLoadValidData(); } catch (...) { std::cout << "FAIL\n"; }
    try { testInvalidSideThrows(); } catch (...) { std::cout << "FAIL\n"; }
    try { testTrimmedValues(); } catch (...) { std::cout << "FAIL\n"; }
    try { testEmptyFile(); } catch (...) { std::cout << "FAIL\n"; }

    std::cout << "\n=== Fin des tests DataLoader ===\n";
}