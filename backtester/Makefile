CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -Iincludes -Isrc

# Répertoires
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Sources de l'application principale
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/data/CSVReader.cpp \
          $(SRC_DIR)/data/DataLoader.cpp \
          $(SRC_DIR)/data/DataConverter.cpp \
          $(SRC_DIR)/data/CSVWriter.cpp

# Sources sans main.cpp (utilisées pour les tests)
LIB_SOURCES = $(SRC_DIR)/data/CSVReader.cpp \
              $(SRC_DIR)/data/DataLoader.cpp \
              $(SRC_DIR)/data/DataConverter.cpp \
              $(SRC_DIR)/data/CSVWriter.cpp

# Fichiers de test avec leur main
TESTSOURCES = $(TEST_DIR)/main.cpp \
              $(TEST_DIR)/data/CSVReaderTest.cpp \
              $(TEST_DIR)/data/CSVWriterTest.cpp \
              $(TEST_DIR)/data/DataLoaderTest.cpp \
              $(TEST_DIR)/data/DataConverterTest.cpp

# Cibles
TARGET = main
TEST_TARGET = runTests

# Compilation du binaire principal
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Compilation des tests unitaires
$(TEST_TARGET): $(LIB_SOURCES) $(TESTSOURCES)
	$(CXX) $(CXXFLAGS) $(LIB_SOURCES) $(TESTSOURCES) -o $(TEST_TARGET)

# Exécution des tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Nettoyage
clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: clean test