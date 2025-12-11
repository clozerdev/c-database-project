TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

CFLAGS = -g -O0 -Wall -Wextra -Iinclude

run: clean default
	./$(TARGET) -f ./employees.db -n
	./$(TARGET) -f ./employees.db

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc $(CFLAGS) -o $@ $^

obj/%.o : src/%.c
	gcc $(CFLAGS) -c $< -o $@

compile_commands:
	bear -- make
