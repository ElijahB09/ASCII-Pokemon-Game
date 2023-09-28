CC = gcc
CFLAGS = -Wall -Wextra -g

# List of source files
SOURCES = main.c minHeap.c queue.c

# List of header files
HEADERS = minHeap.h queue.h

# Generate object file names from source files
OBJECTS = $(SOURCES:.c=.o)

# Output executable name
EXECUTABLE = assignment1.02

# Default target (the first target in the Makefile)
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

