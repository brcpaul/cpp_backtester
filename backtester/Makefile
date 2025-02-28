# Compilateur et options
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Iinclude

# Fichier source et executable
SRC = src/main.cpp
TARGET = backtester

# Regle par defaut
all: $(TARGET)

# Compilation de l’executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Nettoyage
clean:
	rm -f $(TARGET)

# Execution
run: all
	./$(TARGET)

# Indique que ces cibles ne sont pas des fichiers
.PHONY: all clean run