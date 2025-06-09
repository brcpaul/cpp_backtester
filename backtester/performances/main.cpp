#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"
// #include "../includes/core/BookSide.h"
#include <iostream>

void runCSVReaderPerf();
void runDataLoaderPerf();
void runOrderGeneratorPerf();

int main() {
  std::cout << "=== Lancement des tests de performance ===\n\n";

    runCSVReaderPerf();
    runDataLoaderPerf();
    runOrderGeneratorPerf();

  std::cout << "\n=== Tous les tests terminés ===\n";
  return 0;
}
