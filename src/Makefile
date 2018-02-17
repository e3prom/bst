CC=gcc
CFLAGS=-c -Wall
LDFLAGS=

TARGET = bstrings
OBJECTS = $(SOURCES:.c=.o)
SOURCES = bstrings.c

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm $(TARGET) $(OBJECTS)
