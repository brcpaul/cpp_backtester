#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <exception>
#include "../includes/data/CSVReader.h"

// Fonction utilitaire pour créer un fichier de test
void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

// Fonction utilitaire pour nettoyer un fichier
void cleanupFile(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

// Test du constructeur avec un fichier valide
void testConstructorValidFile() {
    std::cout << "Test: Constructor avec fichier valide... ";
    
    std::string testFile = "test_valid.csv";
    createTestFile(testFile, "col1,col2\nval1,val2\n");
    
    try {
        CSVReader reader(testFile);
        std::cout << "PASS\n";
    } catch (...) {
        std::cout << "FAIL\n";
        assert(false);
    }
    
    cleanupFile(testFile);
}

// Test du constructeur avec un fichier inexistant
void testConstructorInvalidFile() {
    std::cout << "Test: Constructor avec fichier inexistant... ";
    
    bool exceptionThrown = false;
    try {
        CSVReader reader("fichier_inexistant.csv");
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    
    assert(exceptionThrown);
    std::cout << "PASS\n";
}

// Test de lecture d'un CSV avec données valides
void testReadCSVValidData() {
    std::cout << "Test: Lecture CSV avec données valides... ";
    
    std::string testFile = "test_data.csv";
    createTestFile(testFile, 
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"
        "1234567890,ORDER_001,AAPL,BUY,LIMIT,100.0,150.25,NEW\n"
        "1234567891,ORDER_002,GOOGL,SELL,MARKET,50.0,,NEW\n"
    );
    
    CSVReader reader(testFile);
    std::vector<CSVRow> rows = reader.readCSV();
    
    // Vérifications
    assert(rows.size() == 3); // Header + 2 lignes de données
    
    // Test de la ligne d'en-tête
    assert(rows[0].values.size() == 8);
    assert(rows[0].values[0] == "timestamp");
    assert(rows[0].values[1] == "order_id");
    assert(rows[0].values[7] == "action");
    
    // Test de la première ligne de données
    assert(rows[1].values.size() == 8);
    assert(rows[1].values[0] == "1234567890");
    assert(rows[1].values[1] == "ORDER_001");
    assert(rows[1].values[2] == "AAPL");
    assert(rows[1].values[6] == "150.25");
    
    // Test ligne avec champ vide (prix vide pour ordre MARKET)
    assert(rows[2].values[6] == "");
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test de lecture d'un fichier CSV vide
void testReadEmptyCSV() {
    std::cout << "Test: Lecture CSV vide... ";
    
    std::string emptyFile = "empty_test.csv";
    createTestFile(emptyFile, "");
    
    CSVReader reader(emptyFile);
    std::vector<CSVRow> rows = reader.readCSV();
    
    assert(rows.size() == 0);
    
    cleanupFile(emptyFile);
    std::cout << "PASS\n";
}

// Test avec des champs vides
void testEmptyFields() {
    std::cout << "Test: Gestion des champs vides... ";
    
    std::string testFile = "empty_fields_test.csv";
    createTestFile(testFile,
        "col1,col2,col3\n"
        "value1,,value3\n"
        ",value2,\n"
    );
    
    CSVReader reader(testFile);
    std::vector<CSVRow> rows = reader.readCSV();
    
    assert(rows.size() == 3);
    assert(rows[1].values[1] == ""); // Champ vide au milieu
    assert(rows[2].values[0] == ""); // Premier champ vide
    assert(rows[2].values[2] == ""); // Dernier champ vide
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

// Test avec des espaces dans les valeurs
void testSpacesInValues() {
    std::cout << "Test: Gestion des espaces... ";
    
    std::string testFile = "spaces_test.csv";
    createTestFile(testFile,
        "col1,col2,col3\n"
        "value with spaces, another value ,third\n"
    );
    
    CSVReader reader(testFile);
    std::vector<CSVRow> rows = reader.readCSV();
    
    assert(rows.size() == 2);
    assert(rows[1].values[0] == "value with spaces");
    assert(rows[1].values[1] == " another value "); // Avec espaces
    
    cleanupFile(testFile);
    std::cout << "PASS\n";
}

int main() {
    std::cout << "=== Tests CSVReader ===\n\n";
    
    try {
        testConstructorValidFile();
        testConstructorInvalidFile();
        testReadCSVValidData();
        testReadEmptyCSV();
        testEmptyFields();
        testSpacesInValues();
        
        std::cout << "\n=== Tous les tests sont passés avec succès! ===\n";
        
    } catch (const std::exception& e) {
        std::cout << "\nErreur lors des tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\nErreur inconnue lors des tests" << std::endl;
        return 1;
    }
    
    return 0;
}