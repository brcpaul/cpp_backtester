#include <iostream>
#include <fstream>
#include <sstream>
#include "../../includes/data/CSVReader.h"

CSVReader::CSVReader(const std::string filename) : filename(filename) {}

std::vector<CSVRow> CSVReader::readCSV() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file: " + filename);
    }

    std::vector<CSVRow> rows;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            // Si la ligne est vide, on ajoute une ligne avec une seule colonne vide
            CSVRow emptyRow;
            emptyRow.values.push_back("");
            rows.push_back(emptyRow);
        } else {
            rows.push_back(split(line, ','));
        }
    }
    return rows;
}

CSVRow CSVReader::split(const std::string &s, char delimiter) {
    std::string token;
    std::stringstream ss(s);
    CSVRow row;

    while (std::getline(ss, token, delimiter)) {
        row.values.push_back(token);
    }

    // Prend en compte le cas où la dernière valeur est vide
    if (!s.empty() && s.back() == delimiter) {
        row.values.push_back("");
    }

    return row;
}