COMPILER=gcc
FLAGS=-Wall -g

OUTPUT=a.out

SRC=src
BUILD=build
BIN=$(BUILD)/$(OUTPUT)





all: $(BIN)

$(BIN): $(SRC)/main.c
	$(COMPILER) $(FLAGS) -lncurses $(SRC)/main.c -o $(BIN)





.PHONY: run clear debug memcheck


run: $(BIN)
	./$(BIN)

clear:
	rm $(BUILD)/*

debug: $(BIN)
	gdb $(BIN)

memcheck: $(BIN)
	valgrind ./$(BIN)