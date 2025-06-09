#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"
// #include "../includes/core/BookSide.h"
#include <iostream>

void runCSVReaderTests();
void runDataLoaderTests();
void runOrderGeneratorTests();
// void runBookSideTests();

int main() {
  std::cout << "=== Lancement des tests ===\n\n";

    runCSVReaderTests();
    runDataLoaderTests();
    runOrderGeneratorTests();
  // runBookSideTests();

  std::cout << "\n=== Tous les tests terminés ===\n";
  return 0;
}
