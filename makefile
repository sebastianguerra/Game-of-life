COMPILER=gcc
FLAGS=-Wall -g

OUTPUT=a.out

SRC=src
BUILD=build
BIN=$(BUILD)/$(OUTPUT)





all: $(BIN)

$(BIN): $(BUILD)/main.o $(BUILD)/game.o
	$(COMPILER) $(BUILD)/main.o $(BUILD)/game.o -lncurses -o $(BIN)


$(BUILD)/main.o: $(BUILD) $(SRC)/main.c $(SRC)/game.h
	$(COMPILER) $(FLAGS) -lncurses $(SRC)/main.c -c -o $(BUILD)/main.o


$(BUILD)/game.o: $(BUILD) $(SRC)/game.c $(SRC)/game.h
	$(COMPILER) $(FLAGS) $(SRC)/game.c -c -o $(BUILD)/game.o


$(BUILD):
	mkdir $(BUILD)


.PHONY: run clear debug memcheck


run: $(BIN)
	./$(BIN)

clear:
	rm -rf $(BUILD)

debug: $(BIN)
	gdb $(BIN)

memcheck: $(BIN)
	valgrind ./$(BIN)
