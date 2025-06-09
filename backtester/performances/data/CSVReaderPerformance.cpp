#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../../includes/data/CSVReader.h"

// Génère un fichier CSV pour les tests de performance du CSVReader
void generateCSVFile(const std::string& filename, size_t rowCount, bool includeEmptyLines = false, bool longLines = false) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Échec de la création du fichier : " + filename);
    }

    file << "col1,col2\n"; // Ligne d’en-tête

    for (size_t i = 0; i < rowCount; ++i) {
        if (includeEmptyLines && i % 100 == 0) {
            file << "\n"; // Ajoute une ligne vide toutes les 100 lignes
        } else {
            if (longLines) {
                std::string longStr(10000, 'x'); // Crée une chaîne de 10 000 'x'
                file << longStr << "," << longStr << "\n";
            } else {
                // Ligne de données classique
                file << "data" << i << "_1,data" << i << "_2\n";
            }
        }
    }
}

// Teste la lecture d’un fichier CSV avec mesure du temps et vérification du nombre de lignes
void testCSVReaderBasic(const std::string& filename, size_t expectedRows) {
    CSVReader reader(filename);

    auto start = std::chrono::high_resolution_clock::now(); // Début du chronomètre
    auto rows = reader.readCSV();                           // Lecture du fichier
    auto end = std::chrono::high_resolution_clock::now();   // Fin du chronomètre

    double timeMs = std::chrono::duration<double, std::milli>(end - start).count(); // Temps en millisecondes

    // Affichage des résultats
    std::cout << "Fichier : " << filename << "\n"
              << "Lignes lues : " << rows.size() << "\n"
              << "Lignes attendues : " << expectedRows << "\n"
              << "Temps : " << timeMs << " ms\n"
              << ((rows.size() == expectedRows) ? "[SUCCÈS]" : "[ÉCHEC]") << "\n\n";
}

// Lance l’ensemble des tests de performance pour le CSVReader
void runCSVReaderPerf() {
    std::cout << "=== Tests de performance CSVReader ===\n\n";
    try {
        // Génération des fichiers de test
        generateCSVFile("test.csv", 100); // 100 lignes + en-tête = 101
        generateCSVFile("test_with_empty_lines.csv", 5000, true); // ~50 lignes vides + 5000 données + en-tête = 5051
        generateCSVFile("test_long_lines.csv", 1000, false, true); // 1000 lignes longues + en-tête = 1001

        // Exécution des tests avec vérification du nombre de lignes attendues
        std::vector<std::pair<std::string, size_t>> tests = {
            {"test.csv", 101},
            {"test_with_empty_lines.csv", 5001},
            {"test_long_lines.csv", 1001}
        };

        for (const auto& [filename, expectedRows] : tests) {
            testCSVReaderBasic(filename, expectedRows);
            if (std::remove(filename.c_str()) != 0) {
                std::cerr << "Erreur lors de la suppression du fichier " << filename << "\n";
            } else {
                std::cout << "Fichier supprimé : " << filename << "\n\n";
            }
        }

        std::cout << "Tous les tests de base sont terminés.\n";
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
    }
    std::cout << "\n=== Fin des tests CSVReader ===\n";
}
