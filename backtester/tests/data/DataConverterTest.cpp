#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include "../includes/data/DataConverter.h"

// Données de test spécifiques au DataConverter
static Data createConverterTestData1() {
    Data testData;
    testData.timestamp = 1234567890123456LL;
    testData.order_id = "ORDER_001";
    testData.instrument = "AAPL";
    testData.side = Side::BUY;
    testData.type = OrderType::LIMIT;
    testData.quantity = 100.0;
    testData.price = 150.25;
    testData.action = Action::NEW;
    return testData;
}

static Data createConverterTestData2() {
    Data testData;
    testData.timestamp = 1234567890123457LL;
    testData.order_id = "ORDER_002";
    testData.instrument = "GOOGL";
    testData.side = Side::SELL;
    testData.type = OrderType::MARKET;
    testData.quantity = 50.0;
    testData.price = 0.0;
    testData.action = Action::MODIFY;
    return testData;
}

static Data createConverterTestData3() {
    Data testData;
    testData.timestamp = 1234567890123458LL;
    testData.order_id = "ORDER_003";
    testData.instrument = "MSFT";
    testData.side = Side::BUY;
    testData.type = OrderType::LIMIT;
    testData.quantity = 200.0;
    testData.price = 300.50;
    testData.action = Action::CANCEL;
    return testData;
}

// Test du constructeur
void testConstructor() {
    std::cout << "Test: Constructor... ";
    
    try {
        DataConverter converter;
        std::cout << "PASS\n";
    } catch (...) {
        std::cout << "FAIL\n";
        assert(false);
    }
}

// Test de conversion d'une donnée simple
void testConvertSingleDataToOutput() {
    std::cout << "Test: Conversion donnée simple... ";
    
    DataConverter converter;
    Data testData = createConverterTestData1();
    OutputData output = converter.convertToOutput(testData);
    
    // Vérifier que les données originales sont correctement copiées
    assert(output.timestamp == testData.timestamp);
    assert(output.order_id == testData.order_id);
    assert(output.instrument == testData.instrument);
    assert(output.side == "BUY");
    assert(output.type == "LIMIT");
    assert(output.quantity == testData.quantity);
    assert(output.price == testData.price);
    assert(output.action == "NEW");
    
    // Vérifier les valeurs par défaut des nouvelles colonnes
    assert(output.status == OrderStatus::PENDING);
    assert(output.executed_quantity == 0.0);
    assert(output.execution_price == 0.0);
    assert(output.counterparty_id == "");
    
    std::cout << "PASS\n";
}

// Test de conversion d'un ordre MARKET
void testConvertMarketOrder() {
    std::cout << "Test: Conversion ordre MARKET... ";
    
    DataConverter converter;
    Data testData = createConverterTestData2();
    OutputData output = converter.convertToOutput(testData);
    
    assert(output.side == "SELL");
    assert(output.type == "MARKET");
    assert(output.action == "MODIFY");
    assert(output.price == 0.0);
    
    std::cout << "PASS\n";
}

// Test de conversion d'une action CANCEL
void testConvertCancelAction() {
    std::cout << "Test: Conversion action CANCEL... ";
    
    DataConverter converter;
    Data testData = createConverterTestData3();
    OutputData output = converter.convertToOutput(testData);
    
    assert(output.action == "CANCEL");
    assert(output.instrument == "MSFT");
    
    std::cout << "PASS\n";
}

// Test de conversion d'un vecteur
void testConvertVectorToOutput() {
    std::cout << "Test: Conversion vecteur... ";
    
    DataConverter converter;
    std::vector<Data> inputVector = {createConverterTestData1(), createConverterTestData2(), createConverterTestData3()};
    std::vector<OutputData> outputVector = converter.convertToOutput(inputVector);
    
    assert(outputVector.size() == 3);
    
    // Vérifier chaque élément
    assert(outputVector[0].order_id == "ORDER_001");
    assert(outputVector[0].side == "BUY");
    assert(outputVector[0].type == "LIMIT");
    assert(outputVector[0].action == "NEW");
    
    assert(outputVector[1].order_id == "ORDER_002");
    assert(outputVector[1].side == "SELL");
    assert(outputVector[1].type == "MARKET");
    assert(outputVector[1].action == "MODIFY");
    
    assert(outputVector[2].order_id == "ORDER_003");
    assert(outputVector[2].side == "BUY");
    assert(outputVector[2].type == "LIMIT");
    assert(outputVector[2].action == "CANCEL");
    
    // Vérifier que tous ont les valeurs par défaut pour les nouvelles colonnes
    for (const auto& output : outputVector) {
        assert(output.status == OrderStatus::PENDING);
        assert(output.executed_quantity == 0.0);
        assert(output.execution_price == 0.0);
        assert(output.counterparty_id == "");
    }
    
    std::cout << "PASS\n";
}

// Test de conversion d'un vecteur vide
void testConvertEmptyVector() {
    std::cout << "Test: Conversion vecteur vide... ";
    
    DataConverter converter;
    std::vector<Data> emptyVector;
    std::vector<OutputData> outputVector = converter.convertToOutput(emptyVector);
    
    assert(outputVector.size() == 0);
    
    std::cout << "PASS\n";
}

// Test des méthodes statiques de conversion
void testSideToString() {
    std::cout << "Test: Conversion Side vers String... ";
    
    assert(DataConverter::sideToString(Side::BUY) == "BUY");
    assert(DataConverter::sideToString(Side::SELL) == "SELL");
    
    // Test avec cast explicite pour tester le cas default
    Side invalidSide = static_cast<Side>(-1);
    assert(DataConverter::sideToString(invalidSide) == "UNKNOWN");
    
    std::cout << "PASS\n";
}

void testOrderTypeToString() {
    std::cout << "Test: Conversion OrderType vers String... ";
    
    assert(DataConverter::orderTypeToString(OrderType::LIMIT) == "LIMIT");
    assert(DataConverter::orderTypeToString(OrderType::MARKET) == "MARKET");
    
    // Test avec cast explicite pour tester le cas default
    OrderType invalidType = static_cast<OrderType>(-1);
    assert(DataConverter::orderTypeToString(invalidType) == "UNKNOWN");
    
    std::cout << "PASS\n";
}

void testActionToString() {
    std::cout << "Test: Conversion Action vers String... ";
    
    assert(DataConverter::actionToString(Action::NEW) == "NEW");
    assert(DataConverter::actionToString(Action::MODIFY) == "MODIFY");
    assert(DataConverter::actionToString(Action::CANCEL) == "CANCEL");
    
    // Test avec cast explicite pour tester le cas default
    Action invalidAction = static_cast<Action>(-1);
    assert(DataConverter::actionToString(invalidAction) == "UNKNOWN");
    
    std::cout << "PASS\n";
}

// Test de préservation de l'intégrité des données
void testPreservesDataIntegrity() {
    std::cout << "Test: Préservation intégrité données... ";
    
    DataConverter converter;
    
    // Test avec des valeurs limites
    Data extremeData;
    extremeData.timestamp = 9223372036854775807LL; // LLONG_MAX
    extremeData.order_id = "";
    extremeData.instrument = "TEST_INSTRUMENT_WITH_LONG_NAME";
    extremeData.side = Side::BUY;
    extremeData.type = OrderType::LIMIT;
    extremeData.quantity = 999999.999999;
    extremeData.price = 0.000001;
    extremeData.action = Action::NEW;
    
    OutputData output = converter.convertToOutput(extremeData);
    
    assert(output.timestamp == extremeData.timestamp);
    assert(output.order_id == extremeData.order_id);
    assert(output.instrument == extremeData.instrument);
    assert(output.quantity == extremeData.quantity);
    assert(output.price == extremeData.price);
    
    std::cout << "PASS\n";
}

// Test avec des caractères spéciaux
void testHandlesSpecialCharacters() {
    std::cout << "Test: Gestion caractères spéciaux... ";
    
    DataConverter converter;
    Data specialData = createConverterTestData1();
    specialData.order_id = "ORDER_WITH_SPECIAL_CHARS_@#$%";
    specialData.instrument = "INST-WITH.DOTS_AND-DASHES";
    
    OutputData output = converter.convertToOutput(specialData);
    
    assert(output.order_id == "ORDER_WITH_SPECIAL_CHARS_@#$%");
    assert(output.instrument == "INST-WITH.DOTS_AND-DASHES");
    
    std::cout << "PASS\n";
}

// Test avec un grand vecteur
void testLargeVector() {
    std::cout << "Test: Conversion grand vecteur... ";
    
    DataConverter converter;
    std::vector<Data> largeVector;
    
    for (int i = 0; i < 100; ++i) {
        Data data = createConverterTestData1();
        data.order_id = "ORDER_" + std::to_string(i);
        largeVector.push_back(data);
    }
    
    std::vector<OutputData> outputVector = converter.convertToOutput(largeVector);
    
    assert(outputVector.size() == 100);
    assert(outputVector[0].order_id == "ORDER_0");
    assert(outputVector[99].order_id == "ORDER_99");
    
    std::cout << "PASS\n";
}

// Fonction principale pour exécuter tous les tests
void runDataConverterTests() {
    std::cout << "=== Tests DataConverter ===\n\n";
    
    // Chaque test encapsulé dans un bloc try pour continuer malgré un échec
    try { testConstructor(); } catch (...) { std::cout << "FAIL\n"; }
    try { testConvertSingleDataToOutput(); } catch (...) { std::cout << "FAIL\n"; }
    try { testConvertMarketOrder(); } catch (...) { std::cout << "FAIL\n"; }
    try { testConvertCancelAction(); } catch (...) { std::cout << "FAIL\n"; }
    try { testConvertVectorToOutput(); } catch (...) { std::cout << "FAIL\n"; }
    try { testConvertEmptyVector(); } catch (...) { std::cout << "FAIL\n"; }
    try { testSideToString(); } catch (...) { std::cout << "FAIL\n"; }
    try { testOrderTypeToString(); } catch (...) { std::cout << "FAIL\n"; }
    try { testActionToString(); } catch (...) { std::cout << "FAIL\n"; }
    try { testPreservesDataIntegrity(); } catch (...) { std::cout << "FAIL\n"; }
    try { testHandlesSpecialCharacters(); } catch (...) { std::cout << "FAIL\n"; }
    try { testLargeVector(); } catch (...) { std::cout << "FAIL\n"; }
    
    std::cout << "\n=== Fin des tests DataConverter ===\n";
}