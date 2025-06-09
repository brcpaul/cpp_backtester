CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -Iincludes -Isrc -fcolor-diagnostics -fansi-escape-codes

# Répertoires
SRC_DIR = src
TEST_DIR = tests
PERF_DIR = performances
BUILD_DIR = build

# Sources de l'application principale
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/data/CSVReader.cpp \
          $(SRC_DIR)/data/DataLoader.cpp \
          $(SRC_DIR)/data/OrderGenerator.cpp \
          $(SRC_DIR)/core/MatchingEngine.cpp \
          $(SRC_DIR)/core/OrderBook.cpp \
          $(SRC_DIR)/core/Order.cpp \
          $(SRC_DIR)/utils/Logger.cpp 


# Sources sans main.cpp (utilisées pour les tests)
LIB_SOURCES = $(SRC_DIR)/data/CSVReader.cpp \
              $(SRC_DIR)/data/DataLoader.cpp \
              $(SRC_DIR)/data/OrderGenerator.cpp \
              $(SRC_DIR)/core/MatchingEngine.cpp \
              $(SRC_DIR)/core/OrderBook.cpp \
              $(SRC_DIR)/core/Order.cpp \
              $(SRC_DIR)/utils/Logger.cpp

# Fichiers de test avec leur main
TESTSOURCES = $(TEST_DIR)/main.cpp \
              $(TEST_DIR)/data/CSVReaderTest.cpp \
              $(TEST_DIR)/data/DataLoaderTest.cpp \
              $(TEST_DIR)/data/OrderGeneratorTest.cpp \
              $(TEST_DIR)/core/MatchingEngineTest.cpp \
              $(TEST_DIR)/core/OrderBookTest.cpp 


# Fichiers de test avec leur main
PERFSOURCES = $(PERF_DIR)/main.cpp \
              $(PERF_DIR)/data/CSVReaderPerformance.cpp \
              $(PERF_DIR)/data/DataLoaderPerformance.cpp \
              $(PERF_DIR)/data/OrderGeneratorPerformance.cpp \
              $(PERF_DIR)/core/MatchingEnginePerformance.cpp \
              $(PERF_DIR)/core/OrderBook.cpp \

# Cibles
TARGET = main
TEST_TARGET = runTests
PERF_TARGET = runPerformanceTests

# Compilation du binaire principal
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Compilation des tests unitaires
$(TEST_TARGET): $(LIB_SOURCES) $(TESTSOURCES)
	$(CXX) $(CXXFLAGS) $(LIB_SOURCES) $(TESTSOURCES) -o $(TEST_TARGET)

# Compilation des tests de performance
$(PERF_TARGET): $(LIB_SOURCES) $(PERFSOURCES)
	$(CXX) $(CXXFLAGS) $(LIB_SOURCES) $(PERFSOURCES) -o $(PERF_TARGET)

# Exécution des tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

performance: $(PERF_TARGET)
	./$(PERF_TARGET)

# Nettoyage
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(PERF_TARGET)

.PHONY: clean test