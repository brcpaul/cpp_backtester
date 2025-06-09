#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <stdexcept>
#include "../includes/data/DataLoader.h"
#include "../utils/TestFile.h"

void testValidOrdersLoad() {
    std::cout << "Test: Chargement d'ordres valides complets... ";

    std::string file = "valid_orders.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,100,150.25,NEW\n"
        "1234567890123456790,124,GOOGL,SELL,MARKET,50,,NEW\n"
        "1234567890123456791,125,MSFT,buy,limit,75, 200.50 ,new\n"  // Test case insensitive + espaces
    );

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.size() == 3);
    
    // Ordre LIMIT
    assert(data[0].order_id == 123);
    assert(data[0].instrument == "AAPL");
    assert(data[0].side == Side::BUY);
    assert(data[0].type == OrderType::LIMIT);
    assert(data[0].price == 150.25);
    
    // Ordre MARKET
    assert(data[1].type == OrderType::MARKET);
    assert(data[1].price == 0.0);  // Prix vide pour MARKET
    
    // Case insensitive + trim
    assert(data[2].side == Side::BUY);
    assert(data[2].type == OrderType::LIMIT);
    assert(data[2].action == Action::NEW);
    assert(data[2].price == 200.50);

    cleanupFile(file);
    std::cout << "PASS\n";
}

void testOrderLifecycleValidation() {
    std::cout << "Test: Validation du cycle de vie des ordres... ";

    std::string file = "order_lifecycle.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,100,AAPL,BUY,LIMIT,100,150.25,NEW\n"
        "1234567890123456790,100,AAPL,BUY,LIMIT,200,160.00,MODIFY\n"
        "1234567890123456791,100,AAPL,BUY,LIMIT,50,155.00,MODIFY\n"
        "1234567890123456792,100,AAPL,BUY,LIMIT,0,,CANCEL\n"
        "1234567890123456793,101,GOOGL,SELL,MARKET,75,,NEW\n"
        "1234567890123456794,101,GOOGL,SELL,MARKET,0,,CANCEL\n"
    );

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.size() == 6);
    
    // Ordre 100: NEW -> MODIFY -> MODIFY -> CANCEL
    assert(data[0].order_id == 100 && data[0].action == Action::NEW);
    assert(data[1].order_id == 100 && data[1].action == Action::MODIFY && data[1].quantity == 200);
    assert(data[2].order_id == 100 && data[2].action == Action::MODIFY && data[2].quantity == 50);
    assert(data[3].order_id == 100 && data[3].action == Action::CANCEL && data[3].quantity == 0);
    
    // Ordre 101: NEW -> CANCEL
    assert(data[4].order_id == 101 && data[4].action == Action::NEW);
    assert(data[5].order_id == 101 && data[5].action == Action::CANCEL);

    cleanupFile(file);
    std::cout << "PASS\n";
}

void testTimestampValidation() {
    std::cout << "Test: Validation des timestamps... ";

    // Timestamp invalide - trop court
    std::string file1 = "timestamp_short.csv";
    createTestFile(file1,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "123456789,123,AAPL,BUY,LIMIT,100,150.25,NEW\n"
    );

    bool thrown = false;
    try {
        DataLoader loader(file1);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("must have exactly 19 digits") != std::string::npos);
    }
    assert(thrown);

    // Timestamp invalide - contient des lettres
    std::string file2 = "timestamp_alpha.csv";
    createTestFile(file2,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "123456789012345678A,123,AAPL,BUY,LIMIT,100,150.25,NEW\n"
    );

    thrown = false;
    try {
        DataLoader loader(file2);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("must contain only digits") != std::string::npos);
    }
    assert(thrown);

    cleanupFile(file1);
    cleanupFile(file2);
    std::cout << "PASS\n";
}

void testOrderIdValidation() {
    std::cout << "Test: Validation des order_id et détection doublons... ";

    // Test doublons NEW (interdit)
    std::string file1 = "duplicate_new.csv";
    createTestFile(file1,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,100,150.25,NEW\n"
        "1234567890123456790,123,GOOGL,SELL,MARKET,50,,NEW\n"
    );

    bool thrown = false;
    try {
        DataLoader loader(file1);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("Duplicate order_id") != std::string::npos);
    }
    assert(thrown);

    // Test order_id invalide (négatif)
    std::string file2 = "negative_order_id.csv";
    createTestFile(file2,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,-123,AAPL,BUY,LIMIT,100,150.25,NEW\n"
    );

    thrown = false;
    try {
        DataLoader loader(file2);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("must contain only digits") != std::string::npos);
    }
    assert(thrown);

    cleanupFile(file1);
    cleanupFile(file2);
    std::cout << "PASS\n";
}

void testMarketVsLimitOrderLogic() {
    std::cout << "Test: Logique MARKET vs LIMIT... ";

    std::string file = "market_limit_logic.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,100,AAPL,BUY,MARKET,100,,NEW\n"           // MARKET sans prix - OK
        "1234567890123456790,101,AAPL,BUY,MARKET,50,150.25,NEW\n"      // MARKET avec prix - prix ignoré
        "1234567890123456791,102,AAPL,SELL,LIMIT,75,200.50,NEW\n"      // LIMIT avec prix - OK
    );

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.size() == 3);
    
    // MARKET sans prix
    assert(data[0].type == OrderType::MARKET);
    assert(data[0].price == 0.0);
    
    // MARKET avec prix (prix ignoré)
    assert(data[1].type == OrderType::MARKET);
    assert(data[1].price == 0.0);  // Prix forcé à 0 pour MARKET
    
    // LIMIT avec prix
    assert(data[2].type == OrderType::LIMIT);
    assert(data[2].price == 200.50);

    // Test LIMIT sans prix (erreur)
    std::string file2 = "limit_no_price.csv";
    createTestFile(file2,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,103,AAPL,BUY,LIMIT,100,,NEW\n"
    );

    bool thrown = false;
    try {
        DataLoader loader2(file2);
        loader2.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("Price is required for LIMIT orders") != std::string::npos);
    }
    assert(thrown);

    cleanupFile(file);
    cleanupFile(file2);
    std::cout << "PASS\n";
}

void testEnumerationValidation() {
    std::cout << "Test: Validation des énumérations... ";

    // Side invalide
    std::string file1 = "invalid_side.csv";
    createTestFile(file1,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,INVALID_SIDE,LIMIT,100,150.25,NEW\n"
    );

    bool thrown = false;
    try {
        DataLoader loader(file1);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("Error in column 'side'") != std::string::npos);
    }
    assert(thrown);

    // Type invalide
    std::string file2 = "invalid_type.csv";
    createTestFile(file2,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,STOP_LOSS,100,150.25,NEW\n"
    );

    thrown = false;
    try {
        DataLoader loader(file2);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("Error in column 'order type'") != std::string::npos);
    }
    assert(thrown);

    // Action invalide
    std::string file3 = "invalid_action.csv";
    createTestFile(file3,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,100,150.25,EXECUTE\n"
    );

    thrown = false;
    try {
        DataLoader loader(file3);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("Error in column 'action'") != std::string::npos);
    }
    assert(thrown);

    cleanupFile(file1);
    cleanupFile(file2);
    cleanupFile(file3);
    std::cout << "PASS\n";
}

void testQuantityAndPriceValidation() {
    std::cout << "Test: Validation quantités et prix... ";

    // Quantité négative
    std::string file1 = "negative_quantity.csv";
    createTestFile(file1,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,-100,150.25,NEW\n"
    );

    bool thrown = false;
    try {
        DataLoader loader(file1);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("must contain only digits") != std::string::npos);
    }
    assert(thrown);

    // Prix invalide (texte)
    std::string file2 = "invalid_price_text.csv";
    createTestFile(file2,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,100,INVALID_PRICE,NEW\n"
    );

    thrown = false;
    try {
        DataLoader loader(file2);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("must be a positive number") != std::string::npos);
    }
    assert(thrown);

    // Prix négatif
    std::string file4 = "invalid_price.csv";
    createTestFile(file4,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,100,-22,NEW\n"
    );

    thrown = false;
    try {
        DataLoader loader(file4);
        loader.loadData();
    } catch (const std::invalid_argument& e) {
        thrown = true;
        assert(std::string(e.what()).find("must be a positive number") != std::string::npos);
    }
    assert(thrown);

    // Quantité zéro pour CANCEL (autorisé)
    std::string file3 = "zero_quantity_cancel.csv";
    createTestFile(file3,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,123,AAPL,BUY,LIMIT,0,,CANCEL\n"
    );

    DataLoader loader(file3);
    std::vector<Data> data = loader.loadData();
    assert(data.size() == 1);
    assert(data[0].quantity == 0);
    assert(data[0].action == Action::CANCEL);

    cleanupFile(file1);
    cleanupFile(file2);
    cleanupFile(file3);
    cleanupFile(file4);
    std::cout << "PASS\n";
}

void testInstrumentAndDataIntegrity() {
    std::cout << "Test: Intégrité des instruments et données... ";

    std::string file = "data_integrity.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890123456789,100, AAPL ,BUY,LIMIT,100,150.25,NEW\n"          // Espaces autour instrument
        "9199999999999999999,999999999,VERY_LONG_INSTRUMENT_NAME,SELL,LIMIT,1000000000,9999.99,NEW\n"  // Valeurs extrêmes
        "1000000000000000000,1,A,BUY,LIMIT,1,0.01,NEW\n"                     // Valeurs minimales
    );

    DataLoader loader(file);
    std::vector<Data> data = loader.loadData();

    assert(data.size() == 3);
    
    // Instrument avec espaces trimés
    assert(data[0].instrument == "AAPL");
    
    // Valeurs extrêmes
    assert(data[1].timestamp == 9199999999999999999LL);
    assert(data[1].order_id == 999999999LL);
    assert(data[1].instrument == "VERY_LONG_INSTRUMENT_NAME");
    assert(data[1].quantity == 1000000000LL);
    assert(data[1].price == 9999.99);
    
    // Valeurs minimales
    assert(data[2].quantity == 1);
    assert(data[2].price == 0.01);

    cleanupFile(file);
    std::cout << "PASS\n";
}

void runDataLoaderTests() {
    std::cout << "=== Tests DataLoader - Responsabilité Validation Métier ===\n\n";

    // Tests de validation métier core
    try { testValidOrdersLoad(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    try { testOrderLifecycleValidation(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    try { testMarketVsLimitOrderLogic(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    
    // Tests de validation des données critiques
    try { testTimestampValidation(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    try { testOrderIdValidation(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    try { testQuantityAndPriceValidation(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    
    // Tests de validation des énumérations
    try { testEnumerationValidation(); } catch (...) { std::cout << "FAIL (exception)\n"; }
    
    // Tests d'intégrité et edge cases
    try { testInstrumentAndDataIntegrity(); } catch (...) { std::cout << "FAIL (exception)\n"; }

    std::cout << "\n=== Fin des tests DataLoader ===\n";
}