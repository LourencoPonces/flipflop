CC = gcc
CFLAGS = -Wall -Werror -pedantic -ansi
TARGET = flipflop

all: $(TARGET)

$(TARGET): flipflop.c
	$(CC) $(CFLAGS) flipflop.c -o $(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all