CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -DDEBUG
INCLUDES = -I./include

DATA_DIR = ./data
DATA_IN = $(DATA_DIR)/main_data
DATA_IN_TESTS = $(DATA_DIR)/tests/input
DATA_OUT_TESTS = $(DATA_DIR)/tests/output
DATA_OUT = $(DATA_DIR)/output
SRC_DIR = ./src
TEST_DIR = ./tests
BUILD_DIR = ./build
BIN_DIR = ./bin

# Source files
LIB_SRC := $(shell find $(SRC_DIR) -name "*.c" ! -name "main.c")
TEST_SRC := $(shell find $(TEST_DIR) -name "*.c" ! -name "*_runner.c")
TEST_RUNNER_SRC := $(TEST_DIR)/tests_runner.c
MAIN_SRC := $(SRC_DIR)/main.c

# Object files
LIB_OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRC))
TEST_RUNNER_OBJ := $(BUILD_DIR)/$(notdir $(TEST_RUNNER_SRC:.c=.o))
MAIN_OBJ := $(BUILD_DIR)/main.o

# Targets
TARGET = $(BIN_DIR)/MatrixLib
TEST_TARGET = $(BIN_DIR)/MatrixLibTest

.PHONY: all test clean rebuild prepare-dirs init-data remove-data run-main

all: prepare-dirs $(TARGET)

test: prepare-dirs $(TEST_TARGET)
	$(TEST_TARGET)

rebuild: clean all

prepare-dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)
	@mkdir -p $(dir $(LIB_OBJ)) $(dir $(TEST_OBJ))

init-data:
	@mkdir -p $(DATA_DIR) $(DATA_OUT) $(DATA_IN)
	@printf "2 2\n1.0 2.0\n4.0 5.0" > $(DATA_IN)/Matrix_A.txt
	@printf "2 3\n1.0 2.0 3.0\n4.0 5.0 6.0" > $(DATA_IN)/Matrix_B.txt
	@printf "3 2\n1.0 1.0\n1.0 1.0\n 1.0 1.0" > $(DATA_IN)/Matrix_C.txt
	@printf "2 2\n0.5 0.5\n0.5 0.5" > $(DATA_IN)/Matrix_D.txt
	@echo "Тестовые данные созданы в $(DATA_IN)"

init-test-data:
	@mkdir -p $(DATA_DIR)/input $(DATA_DIR)/output
	@echo "2 2\n1 2\n3 4" > $(DATA_DIR)/input/A.txt
	@echo "2 2\n5 6\n7 8" > $(DATA_DIR)/input/B.txt
	@echo "2 2\n1 1\n1 1" > $(DATA_DIR)/input/C.txt
	@echo "2 2\n1 2\n4 5" > $(DATA_DIR)/input/D.txt
	@echo "Тестовые данные созданы в $(DATA_DIR)/input/"

remove-data:
	rm -rf $(DATA_IN)

$(TARGET): $(LIB_OBJ) $(MAIN_OBJ)$(remove-data)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_TARGET): $(LIB_OBJ) $(TEST_OBJ) $(TEST_RUNNER_OBJ) | prepare-dirs
	$(CC) $(CFLAGS) $^ -o $@ -lcunit

$(BUILD_TEST_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(DATA_DIR)

run-main: init-data all
	$(TARGET)
	make remove-data

check-format:
	@echo "Checking code style..."
	@find "./" -name '*.c' -o -name '*.h' | \
		xargs clang-format --style=file --dry-run -Werror

format:
	@echo "Reformatting code..."
	@find "./" -name '*.c' -o -name '*.h' | \
		xargs clang-format --style=file -i -Werror
