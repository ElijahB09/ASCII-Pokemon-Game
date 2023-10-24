CC = gcc
CFLAGS = -Wall -Werror -g -lncurses

# List of source files
SOURCES = main.c minHeap.c queue.c neighbors.c buildings.c dijkstra.c worldgen.c handleTurns.c handleBattle.c

# List of header files
HEADERS = minHeap.h queue.h neigbors.h buildings.h dijkstra.h worldgen.h handleTurns.h structs.h handleBattle.h

# Generate object file names from source files
OBJECTS = $(SOURCES:.c=.o)

# Output executable name
EXECUTABLE = PokeGame

# Default target (the first target in the Makefile)
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

