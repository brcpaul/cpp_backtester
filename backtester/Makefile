CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra
INCLUDES = -Iincludes -Isrc
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/data/CSVReader.cpp $(SRCDIR)/data/DataLoader.cpp
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: clean