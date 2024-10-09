CC = gcc
CFLAGS = -std=c99 -Wall -O3 -g -pthread -Wextra
LIBS = -lXrandr -lXi -lXxf86vm -lpthread -ldl -lm -lglfw -lGL
LDFLAGS = lib/glad/src/glad.o

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin
TARGET = $(BIN)/game

.PHONY: all clean

all: dir libs $(TARGET) run

dir:
	mkdir -p $(BIN)

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BIN) $(OBJ)
	cd lib/glad/src && rm glad.o
