#include <iostream>

void runCSVReaderTests();
void runDataLoaderTests();
void runCSVWriterTests();
void runDataConverterTests();

int main() {
    std::cout << "=== Lancement des tests ===\n\n";
    
    runCSVReaderTests();
    runDataLoaderTests();
    runCSVWriterTests();
    runDataConverterTests();
    
    std::cout << "\n=== Tous les tests terminés ===\n";
    return 0;
}
