#include <iostream>
#include <fstream>
#include <sstream>
#include "../../includes/data/CSVReader.h"

CSVReader::CSVReader(const std::string filename) : filename(filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("File not found: " + filename);
    }
}

std::vector<CSVRow> CSVReader::readCSV()
{
    std::vector<CSVRow> rows;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        rows.push_back(split(line, ','));
    }
    return rows;
}

CSVRow CSVReader::split(const std::string &s, char delimiter)
{
    std::string token;
    std::stringstream ss(s);
    CSVRow row;

    while (std::getline(ss, token, delimiter))
    {
        row.values.push_back(token);
    }

    // Prend en compte le cas où la dernière valeur est vide
    if (!s.empty() && s.back() == delimiter)
    {
        row.values.push_back("");
    }

    return row;
}