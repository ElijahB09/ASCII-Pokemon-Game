CC = gcc
CFLAGS = -Wall -Werror -g

# List of source files
SOURCES = main.c minHeap.c queue.c neighbors.c buildings.c dijkstra.c worldgen.c handleTurns.c

# List of header files
HEADERS = minHeap.h queue.h neigbors.h buildings.h dijkstra.h worldgen.h handleTurns.h structs.h

# Generate object file names from source files
OBJECTS = $(SOURCES:.c=.o)

# Output executable name
EXECUTABLE = assignment1.04

# Default target (the first target in the Makefile)
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

