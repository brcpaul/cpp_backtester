#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <exception>
#include "../includes/data/CSVReader.h"
#include "../utils/TestFile.h"


void testBasicParsing() {
    std::cout << "Test: Parsing de base et header seul... ";
    
    // Cas normal avec données
    std::string file1 = "basic_parsing.csv";
    createTestFile(file1, "col1,col2,col3\nval1,val2,val3\ndata1,data2,data3\n");
    
    CSVReader reader1(file1);
    auto rows1 = reader1.readCSV();
    assert(rows1.size() == 3);
    assert(rows1[0].values[0] == "col1");
    assert(rows1[1].values[2] == "val3");
    assert(rows1[2].values[1] == "data2");
    
    // Header seul 
    std::string file2 = "header_only.csv";
    createTestFile(file2, "timestamp,order_id,instrument,side,type,quantity,price,action\n");
    
    CSVReader reader2(file2);
    auto rows2 = reader2.readCSV();
    assert(rows2.size() == 1);
    assert(rows2[0].values.size() == 8);
    
    cleanupFile(file1);
    cleanupFile(file2);
    std::cout << "PASS\n";
}

void testFieldDelimiterHandling() {
    std::cout << "Test: Gestion des délimiteurs et champs vides... ";
    
    std::string file = "delimiter_test.csv";
    createTestFile(file,
        "col1,col2,col3,col4,\n"           // Virgule finale
        "value1,,value3,,\n"               // Champs vides au milieu et fin
        ",empty_first,value3,\n"           // Premier champ vide
        "single\n"                         // Une seule valeur (pas de virgule)
    );
    
    CSVReader reader(file);
    auto rows = reader.readCSV();
    
    assert(rows.size() == 4);
    
    // Ligne 1: virgule finale crée un champ vide
    assert(rows[0].values.size() == 5);
    assert(rows[0].values[4] == "");
    
    // Ligne 2: champs vides préservés
    assert(rows[1].values[1] == "");
    assert(rows[1].values[3] == "");
    
    // Ligne 3: premier champ vide
    assert(rows[2].values[0] == "");
    assert(rows[2].values[1] == "empty_first");
    
    // Ligne 4: aucune virgule
    assert(rows[3].values.size() == 1);
    assert(rows[3].values[0] == "single");
    
    cleanupFile(file);
    std::cout << "PASS\n";
}

void testInconsistentColumnCounts() {
    std::cout << "Test: Lignes avec nombres de colonnes variables... ";
    
    std::string file = "inconsistent_columns.csv";
    createTestFile(file,
        "timestamp,order_id,instrument,side,type,quantity,price,action\n"  // 8 colonnes
        "123,456,AAPL,BUY\n"                                              // 4 colonnes
        "789,012,GOOGL,SELL,LIMIT,100,150.25,NEW,EXTRA\n"                 // 9 colonnes
        "single_value\n"                                                   // 1 colonne
    );
    
    CSVReader reader(file);
    auto rows = reader.readCSV();
    
    assert(rows.size() == 4);
    assert(rows[0].values.size() == 8);  // Header
    assert(rows[1].values.size() == 4);  // Ligne courte
    assert(rows[2].values.size() == 9);  // Ligne longue
    assert(rows[3].values.size() == 1);  // Une seule valeur
    
    cleanupFile(file);
    std::cout << "PASS\n";
}

void testSpecialCharactersAndSpaces() {
    std::cout << "Test: Caractères spéciaux et espaces... ";
    
    std::string file = "special_chars.csv";
    createTestFile(file,
        "col1,col2,col3\n"
        " leading_space,trailing_space ,both \n"
        "tab\there,new,comma\\,here\n"
        "\"quoted\",normal,end\n"
    );
    
    CSVReader reader(file);
    auto rows = reader.readCSV();
    assert(rows.size() == 4);
    
    // Vérification que les espaces sont préservés 
    assert(rows[1].values[0] == " leading_space");
    assert(rows[1].values[1] == "trailing_space ");
    assert(rows[1].values[2] == "both ");
    
    // Caractères spéciaux préservés
    assert(rows[2].values[0].find('\t') != std::string::npos);
    
    cleanupFile(file);
    std::cout << "PASS\n";
}

void testFileAccessErrors() {
    std::cout << "Test: Erreurs d'accès fichier... ";
    
    // Fichier inexistant
    bool exceptionThrown = false;
    try {
        CSVReader reader("nonexistent_file.csv");
        reader.readCSV();
    } catch (const std::runtime_error& e) {
        exceptionThrown = true;
        std::string msg = e.what();
        assert(msg.find("Failed to open CSV file") != std::string::npos);
    }
    assert(exceptionThrown);
    
    // Fichier complètement vide
    std::string emptyFile = "completely_empty.csv";
    createTestFile(emptyFile, "");
    
    CSVReader reader2(emptyFile);
    auto rows = reader2.readCSV();
    assert(rows.empty());
    
    cleanupFile(emptyFile);
    std::cout << "PASS\n";
}

void testLargeDataParsing() {
    std::cout << "Test: Parsing de données volumineuses... ";
    
    std::string file = "large_data.csv";
    std::string content = "timestamp,order_id,instrument,side,type,quantity,price,action\n";
    
    // Génère 1000 lignes de données
    for (int i = 1; i <= 1000; ++i) {
        content += "123456789012345678" + std::to_string(i) + "," + 
                   std::to_string(i) + ",AAPL,BUY,LIMIT,100,150.25,NEW\n";
    }
    
    createTestFile(file, content);
    
    CSVReader reader(file);
    auto rows = reader.readCSV();
    
    assert(rows.size() == 1001);  // Header + 1000 lignes
    assert(rows[1].values[1] == "1");      // Premier ordre
    assert(rows[1000].values[1] == "1000"); // Dernier ordre
    
    cleanupFile(file);
    std::cout << "PASS\n";
}

void runCSVReaderTests() {
    std::cout << "=== Tests CSVReader - Responsabilité Parsing ===\n\n";

    try { testBasicParsing(); } catch (...) { std::cout << "FAIL\n"; }
    try { testFieldDelimiterHandling(); } catch (...) { std::cout << "FAIL\n"; }
    try { testInconsistentColumnCounts(); } catch (...) { std::cout << "FAIL\n"; }
    try { testSpecialCharactersAndSpaces(); } catch (...) { std::cout << "FAIL\n"; }
    try { testFileAccessErrors(); } catch (...) { std::cout << "FAIL\n"; }
    try { testLargeDataParsing(); } catch (...) { std::cout << "FAIL\n"; }

    std::cout << "\n=== Fin des tests CSVReader ===\n";
}