#include "../includes/core/MatchingEngine.h"
#include "../includes/core/Order.h"
// #include "../includes/core/BookSide.h"
#include <iostream>

void runCSVReaderTests();
void runDataLoaderTests();
void runCSVWriterTests();
void runDataConverterTests();
void runBookSideTests();

int main() {
  std::cout << "=== Lancement des tests ===\n\n";

    runCSVReaderTests();
    runDataLoaderTests();
  // runBookSideTests();

  std::cout << "\n=== Tous les tests terminés ===\n";
  return 0;
}
