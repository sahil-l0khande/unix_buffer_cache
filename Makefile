# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude 
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Target binary
TARGET = $(BIN_DIR)/buffer_cache

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Default target
all: $(TARGET)

# Rule to build the target binary
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Rule to compile source files into object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean

