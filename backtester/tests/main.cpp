#include <iostream>

void runCSVReaderTests();
void runDataLoaderTests();
void runOrderGeneratorTests();
void runMatchingEngineTests();
void runOrderBookTests();

int main() {
  std::cout << "=== Lancement des tests ===\n\n";

    runCSVReaderTests();
    runDataLoaderTests();
    runOrderGeneratorTests();
    runMatchingEngineTests();
    runOrderBookTests();

  std::cout << "\n=== Tous les tests terminés ===\n";
  return 0;
}
