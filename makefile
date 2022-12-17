COMPILER=gcc
FLAGS=-Wall -g
NAME=a.out

all: a.out


a.out: src/main.c
	$(COMPILER) $(FLAGS) src/main.c -o $(NAME)


.PHONY: run clear debug memcheck


run: $(NAME)
	./$(NAME)

clear:
	rm *.o $(NAME)

debug: $(NAME)
	gdb $(NAME)

memcheck: $(NAME)
	valgrind ./$(NAME)