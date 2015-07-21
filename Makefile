TARGET=enigma
CC=gcc
CFLAGS=-Wall -O3 -pedantic -std=c99
OBJS=main.o enigma.o

all: $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm $(OBJS)

mrproper: clean
	rm $(TARGET)

