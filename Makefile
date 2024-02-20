# Copyright 2024, Gabriel Chiong <gabrielchiong@g.harvard.edu>
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

# Test source directory and executable name
TEST_SRC_DIR := tests
TEST_OBJ_DIR := $(OBJ_DIR)/tests
TEST_BIN := $(BIN_DIR)/test_app

# Examples source directory and executable name
EXAMPLES_SRC_DIR := examples
EXAMPLE_BIN := $(BIN_DIR)/example_app

# Target executable name
TARGET := $(BIN_DIR)/my_app

# Automatically list all source files in the SRC_DIR
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object file names in the OBJ_DIR
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Automatically list all test source files in the TEST_SRC_DIR
TEST_SRCS := $(wildcard $(TEST_SRC_DIR)/*.cpp)

# Generate corresponding test object file names in the TEST_OBJ_DIR
TEST_OBJS := $(TEST_SRCS:$(TEST_SRC_DIR)/%.cpp=$(TEST_OBJ_DIR)/%.o)

# Automatically list all example source files in the EXAMPLES_SRC_DIR
EXAMPLE_SRCS := $(wildcard $(EXAMPLES_SRC_DIR)/*.cpp)

# Generate corresponding example object file names in the OBJ_DIR
EXAMPLE_OBJS := $(EXAMPLE_SRCS:$(EXAMPLES_SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default rule
all: $(TARGET)

# Rule to make the target
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test rule
.PHONY: tests
tests: $(TEST_BIN)
	./$(TEST_BIN)

# Rule to compile and run the example
.PHONY: examples
examples: $(EXAMPLE_BIN)
	./$(EXAMPLE_BIN)

# Rule to make the test binary
$(TEST_BIN): $(TEST_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile test sources into object files
$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to make the example binary
$(EXAMPLE_BIN): $(EXAMPLE_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Create binary, object, and test object directories if they don't exist
$(BIN_DIR) $(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

# Rule to compile example sources into object files
$(OBJ_DIR)/%.o: $(EXAMPLES_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule for removing build artifacts
clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(TEST_OBJ_DIR)

# Prevent make from treating these as file names
.PHONY: all clean
