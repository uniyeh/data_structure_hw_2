# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Target executable
TARGET = main

# Source files
SOURCES = main.cpp
HEADERS = BST.h AVL.h Treap.h SkipList.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all run clean rebuild
