CC = gcc
CFLAGS = -Wall -Werror -pedantic -ansi
TARGET = flipflop

all: $(TARGET)

clean:
	rm -f flipflop

$(TARGET): flipflop.c
	$(CC) $(CFLAGS) flipflop.c -o $(TARGET)


rebuild: clean all