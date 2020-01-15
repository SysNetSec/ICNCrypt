SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:.c=.o)
	
DEBUG_OBJECTS=$(SOURCES:.c=_debug.o)
NOHASH_OBJECTS=$(SOURCES:.c=_nohash.o)

TARGET=huffman
TARGETNOHASH=huffman_nohmac
DEBUG_TARGET=huffman_debug

NOHASHFLAGS=-DNOHASH
HASHFLAGS=-DHASH
CCFLAGS= -Wpedantic -O4
LDFLAGS=-lcrypto

DEBUG_FLAGS=-DDEBUG -g

.PHONY: all debug clean

all: $(TARGET) $(TARGETNOHASH)
debug: $(DEBUG_TARGET)

clean:
	rm -f $(DEBUG_TARGET) $(OBJECTS) $(DEBUG_OBJECTS) $(TARGETNOHASH) $(NOHASH_OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGETNOHASH): $(NOHASH_OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%_debug.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

%_nohash.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) $(NOHASHFLAGS) -o $@ -c $<

%.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) $(HASHFLAGS) -o $@ -c $<
