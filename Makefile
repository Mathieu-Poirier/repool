# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++26 -Wall -Wextra -Iheaders

# Directories
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build

# Sources
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
SRC_OBJS_NO_MAIN := $(filter-out $(BUILD_DIR)/main.o, $(SRC_OBJS))

# Tests
TEST_FILES := $(wildcard $(TEST_DIR)/*_test.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%, $(TEST_FILES))

# Main binary
TARGET := $(BUILD_DIR)/main

.PHONY: all check clean

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link main executable
$(TARGET): $(BUILD_DIR) $(SRC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_OBJS)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile individual test binaries and link with source code (excluding main.o)
$(BUILD_DIR)/%_test: $(TEST_DIR)/%_test.cpp $(SRC_OBJS_NO_MAIN) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< $(SRC_OBJS_NO_MAIN) -o $@

# Run all tests
check: $(TEST_BINS)
	@echo "TESTS:"
	@for test in $(TEST_BINS); do echo ">>> $$test"; ./$$test || exit 1; done

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
