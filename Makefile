.SILENT:

CC = gcc
CFLAGS = $(shell pkg-config --cflags sdl3 sdl3-ttf)
LDFLAGS = $(shell pkg-config --libs sdl3 sdl3-ttf)
TARGET = main
SOURCES = main.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
