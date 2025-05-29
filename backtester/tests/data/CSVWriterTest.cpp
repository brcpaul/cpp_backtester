#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../includes/data/CSVWriter.h"
#include "../utils/TestFile.h"

// Fonction utilitaire pour lire le contenu d'un fichier
std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Fonction utilitaire pour diviser le contenu en lignes
std::vector<std::string> splitLines(const std::string& content) {
    std::vector<std::string> lines;
    std::stringstream ss(content);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

// Fonction utilitaire pour créer des données de test
OutputData createTestData1() {
    OutputData data;
    data.timestamp = 1234567890123456LL;
    data.order_id = "ORDER_001";
    data.instrument = "AAPL";
    data.side = "BUY";
    data.type = "LIMIT";
    data.quantity = 100.0;
    data.price = 150.25;
    data.action = "NEW";
    data.status = OrderStatus::EXECUTED;
    data.executed_quantity = 100.0;
    data.execution_price = 150.30;
    data.counterparty_id = "COUNTER_001";
    return data;
}

OutputData createTestData2() {
    OutputData data;
    data.timestamp = 1234567890123457LL;
    data.order_id = "ORDER_002";
    data.instrument = "GOOGL";
    data.side = "SELL";
    data.type = "MARKET";
    data.quantity = 50.0;
    data.price = 0.0;
    data.action = "MODIFY";
    data.status = OrderStatus::PARTIALLY_EXECUTED;
    data.executed_quantity = 25.0;
    data.execution_price = 2800.50;
    data.counterparty_id = "COUNTER_002";
    return data;
}

// Test du constructeur avec fichier valide
void testCSVWriterConstructorValidFile() {
    std::cout << "Test: Constructor avec fichier valide... ";
    
    std::string testFile = "test_output.csv";
    
    try {
        CSVWriter writer(testFile);
        std::cout << "PASS\n";
    } catch (...) {
        std::cout << "FAIL\n";
        assert(false);
    }
    
    cleanupFile(testFile);
}

// Test du constructeur avec chemin invalide
void testCSVWriterConstructorInvalidPath() {
    std::cout << "Test: Constructor avec chemin invalide... ";
    
    bool exceptionThrown = false;
    try {
        CSVWriter writer("/invalid_path/test.csv");
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    
    assert(exceptionThrown);
    std::cout << "PASS\n";
}

// Test d'écriture du header
void testWriteHeader() {
    std::cout << "Test: Écriture du header... ";
    
    std::string testFile = "test_header.csv";
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.close();
    
    std::string content = readFileContent(testFile);
    std::vector<std::string> lines = splitLines(content);
    
    assert(lines.size() >= 1);
    assert(lines[0] == "timestamp,order_id,instrument,side,type,quantity,price,action,status,executed_quantity,execution_price,counterparty_id");
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test d'écriture d'une seule donnée
void testWriteSingleData() {
    std::cout << "Test: Écriture d'une donnée... ";
    
    std::string testFile = "test_single.csv";
    OutputData testData = createTestData1();
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(testData);
    writer.close();
    
    std::string content = readFileContent(testFile);
    std::vector<std::string> lines = splitLines(content);
    
    assert(lines.size() == 2); // Header + 1 ligne de données
    
    std::string expectedLine = "1234567890123456,ORDER_001,AAPL,BUY,LIMIT,100.000000,150.250000,NEW,EXECUTED,100.000000,150.300000,COUNTER_001";
    assert(lines[1] == expectedLine);
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test d'écriture de plusieurs données séparément
void testWriteMultipleData() {
    std::cout << "Test: Écriture de plusieurs données... ";
    
    std::string testFile = "test_multiple.csv";
    OutputData testData1 = createTestData1();
    OutputData testData2 = createTestData2();
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(testData1);
    writer.writeData(testData2);
    writer.close();
    
    std::string content = readFileContent(testFile);
    std::vector<std::string> lines = splitLines(content);
    
    assert(lines.size() == 3); // Header + 2 lignes de données
    assert(lines[1].find("ORDER_001") != std::string::npos);
    assert(lines[1].find("EXECUTED") != std::string::npos);
    assert(lines[2].find("ORDER_002") != std::string::npos);
    assert(lines[2].find("PARTIALLY_EXECUTED") != std::string::npos);
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test d'écriture d'un vecteur de données
void testWriteVectorData() {
    std::cout << "Test: Écriture d'un vecteur de données... ";
    
    std::string testFile = "test_vector.csv";
    std::vector<OutputData> dataVector = {createTestData1(), createTestData2()};
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(dataVector);
    writer.close();
    
    std::string content = readFileContent(testFile);
    std::vector<std::string> lines = splitLines(content);
    
    assert(lines.size() == 3); // Header + 2 lignes de données
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test d'écriture d'un vecteur vide
void testWriteEmptyVector() {
    std::cout << "Test: Écriture d'un vecteur vide... ";
    
    std::string testFile = "test_empty_vector.csv";
    std::vector<OutputData> emptyVector;
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(emptyVector);
    writer.close();
    
    std::string content = readFileContent(testFile);
    std::vector<std::string> lines = splitLines(content);
    
    assert(lines.size() == 1); // Seulement le header
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test de tous les statuts de commande
void testAllOrderStatuses() {
    std::cout << "Test: Tous les statuts de commande... ";
    
    std::string testFile = "test_statuses.csv";
    std::vector<OutputData> statusTestData;
    
    OutputData data = createTestData1();
    data.status = OrderStatus::EXECUTED;
    statusTestData.push_back(data);
    
    data.status = OrderStatus::PARTIALLY_EXECUTED;
    statusTestData.push_back(data);
    
    data.status = OrderStatus::PENDING;
    statusTestData.push_back(data);
    
    data.status = OrderStatus::CANCELED;
    statusTestData.push_back(data);
    
    data.status = OrderStatus::REJECTED;
    statusTestData.push_back(data);
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(statusTestData);
    writer.close();
    
    std::string content = readFileContent(testFile);
    
    assert(content.find("EXECUTED") != std::string::npos);
    assert(content.find("PARTIALLY_EXECUTED") != std::string::npos);
    assert(content.find("PENDING") != std::string::npos);
    assert(content.find("CANCELED") != std::string::npos);
    assert(content.find("REJECTED") != std::string::npos);
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test du formatage de précision
void testPrecisionFormatting() {
    std::cout << "Test: Formatage de précision... ";
    
    std::string testFile = "test_precision.csv";
    OutputData precisionData = createTestData1();
    precisionData.quantity = 123.123456789;
    precisionData.price = 456.987654321;
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(precisionData);
    writer.close();
    
    std::string content = readFileContent(testFile);
    
    // Vérifier que les nombres sont formatés avec 6 décimales
    assert(content.find("123.123457") != std::string::npos); // Arrondi à 6 décimales
    assert(content.find("456.987654") != std::string::npos);
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test de gestion des valeurs zéro
void testZeroValues() {
    std::cout << "Test: Gestion des valeurs zéro... ";
    
    std::string testFile = "test_zero.csv";
    OutputData zeroData = createTestData1();
    zeroData.quantity = 0.0;
    zeroData.price = 0.0;
    zeroData.executed_quantity = 0.0;
    zeroData.execution_price = 0.0;
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(zeroData);
    writer.close();
    
    std::string content = readFileContent(testFile);
    assert(content.find("0.000000") != std::string::npos);
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test de gestion des chaînes vides
void testEmptyStrings() {
    std::cout << "Test: Gestion des chaînes vides... ";
    
    std::string testFile = "test_empty_strings.csv";
    OutputData emptyData = createTestData1();
    emptyData.order_id = "";
    emptyData.instrument = "";
    emptyData.counterparty_id = "";
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(emptyData);
    writer.close();
    
    std::string content = readFileContent(testFile);
    std::vector<std::string> lines = splitLines(content);
    
    assert(lines.size() == 2);
    
    // Vérifier que les champs vides sont présents (virgules consécutives)
    std::string dataLine = lines[1];
    size_t commaCount = std::count(dataLine.begin(), dataLine.end(), ',');
    assert(commaCount == 11); // 12 colonnes = 11 virgules
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test de gestion d'un statut invalide
void testInvalidOrderStatus() {
    std::cout << "Test: Gestion d'un statut invalide... ";
    
    std::string testFile = "test_invalid_status.csv";
    OutputData invalidStatusData = createTestData1();
    invalidStatusData.status = static_cast<OrderStatus>(-1); // Statut invalide
    
    CSVWriter writer(testFile);
    writer.writeHeader();
    writer.writeData(invalidStatusData);
    writer.close();
    
    std::string content = readFileContent(testFile);
    assert(content.find("UNKNOWN") != std::string::npos);
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Fonction principale pour exécuter tous les tests
void runCSVWriterTests() {
    std::cout << "=== Tests CSVWriter ===\n\n";

    try { testCSVWriterConstructorValidFile(); } catch (...) { std::cout << "FAIL\n"; }
    try { testCSVWriterConstructorInvalidPath(); } catch (...) { std::cout << "FAIL\n"; }
    try { testWriteHeader(); } catch (...) { std::cout << "FAIL\n"; }
    try { testWriteSingleData(); } catch (...) { std::cout << "FAIL\n"; }
    try { testWriteMultipleData(); } catch (...) { std::cout << "FAIL\n"; }
    try { testWriteVectorData(); } catch (...) { std::cout << "FAIL\n"; }
    try { testWriteEmptyVector(); } catch (...) { std::cout << "FAIL\n"; }
    try { testAllOrderStatuses(); } catch (...) { std::cout << "FAIL\n"; }
    try { testPrecisionFormatting(); } catch (...) { std::cout << "FAIL\n"; }
    try { testZeroValues(); } catch (...) { std::cout << "FAIL\n"; }
    try { testEmptyStrings(); } catch (...) { std::cout << "FAIL\n"; }
    try { testInvalidOrderStatus(); } catch (...) { std::cout << "FAIL\n"; }

    std::cout << "\n=== Fin des tests CSVWriter ===\n";
}