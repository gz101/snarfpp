# Copyright 2024, Gabriel Chiong <gabriel.chiong@g.harvard.edu>
# See LICENSE in the directory root for terms of use.

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude -O3

# Source directory
SRC_DIR := src

# Object files directory
OBJ_DIR := build

# Binary directory
BIN_DIR := bin

# Target executable name
TARGET := $(BIN_DIR)/my_app

# Automatically list all source files in the SRC_DIR
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object file names in the OBJ_DIR
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default rule
all: $(TARGET)

# Rule to make the target
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create binary and object directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean rule for removing build artifacts
clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR)

# Prevent make from treating these as file names
.PHONY: all clean
