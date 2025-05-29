#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <fstream>
#include <filesystem>

// Fonction utilitaire pour créer un fichier de test
inline void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

// Fonction utilitaire pour nettoyer un fichier
inline void cleanupFile(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

#endif