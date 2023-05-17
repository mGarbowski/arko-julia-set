CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lX11

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = empty_window

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
