#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <string>
#include <vector>

struct CSVRow {
    std::vector<std::string> values;
};

class CSVReader {

public:
    CSVReader(const std::string filename);
    std::vector<CSVRow> readCSV();

private:
    CSVRow split(const std::string &s, char delimiter);
    std::string filename;
};

#endif // CSVREADER_H

