CCFLAGS = -std=c11 -Wall -Wl,--no-as-needed -ldl
CC = gcc
SOURCES_DIR = src
TESTS_DIR = tests
BIN_DIR = bin
TESTS_BIN_DIR = tests_bin
INCLUDE_DIR = include
SOURCES = $(wildcard $(SOURCES_DIR)/*.c)

all: dllist

run: all
	./bin/dllist

test: build_tests run_tests

build_tests: $(TESTS)
	rm -rf $(TESTS_BIN_DIR)
	mkdir $(TESTS_BIN_DIR)
	echo "Building unit tests..."
	gcc $(CCFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/TestListEmplaceBack.c $(SOURCES_DIR)/StringList.c -o $(TESTS_BIN_DIR)/TestListEmplaceBack
	gcc $(CCFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/TestListEmplaceFront.c $(SOURCES_DIR)/StringList.c -o $(TESTS_BIN_DIR)/TestListEmplaceFront
	gcc $(CCFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/TestListCompare.c $(SOURCES_DIR)/StringList.c -o $(TESTS_BIN_DIR)/TestListCompare
	gcc $(CCFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/TestListSort.c $(SOURCES_DIR)/StringList.c -o $(TESTS_BIN_DIR)/TestListSort
	gcc $(CCFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/TestListCompress.c $(SOURCES_DIR)/StringList.c -o $(TESTS_BIN_DIR)/TestListCompress

run_tests:
	echo "Running unit tests..."
	./$(TESTS_BIN_DIR)/TestListEmplaceBack && echo "PASS!" || echo "FAIL!"
	./$(TESTS_BIN_DIR)/TestListEmplaceFront && echo "PASS!" || echo "FAIL!"
	./$(TESTS_BIN_DIR)/TestListCompare && echo "PASS!" || echo "FAIL!"
	./$(TESTS_BIN_DIR)/TestListSort && echo "PASS!" || echo "FAIL!"
	./$(TESTS_BIN_DIR)/TestListCompress && echo "PASS!" || echo "FAIL!"

debug: CCFLAGS += -DDEBUG -g
debug: all

dllist: $(SOURCES)
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)
	gcc $(CCFLAGS) -I$(INCLUDE_DIR) $^ -o $(BIN_DIR)/$@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(TESTS_BIN_DIR)