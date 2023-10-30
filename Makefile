CXX = g++
CXXFLAGS = -Wall -Werror -g -lncurses

# List of source files
SOURCES = main.cpp minHeap.cpp queue.cpp neighbors.cpp buildings.cpp dijkstra.cpp worldgen.cpp handleTurns.cpp handleBattle.cpp

# List of header files
HEADERS = minHeap.h queue.h neighbors.h buildings.h dijkstra.h worldgen.h handleTurns.h structs.h handleBattle.h

# Generate object file names from source files
OBJECTS = $(SOURCES:.cpp=.o)

# Output executable name
EXECUTABLE = PokeGame

# Default target (the first target in the Makefile)
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
