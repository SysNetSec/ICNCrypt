CC=gcc
CCFLAGS=-O4 -Wno-deprecated
FLAGS=-lcrypto 
SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:.c=.o)

TARGET=aes

all: $(TARGET)

huffman: main.cpp huffman.h
	$(CC) $(CCFLAGS) main.cpp -o huffman $(FLAGS)

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^ $(FLAGS)

clean:
	rm *.o
	rm aes
