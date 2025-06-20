CXX = g++
CXXFLAGS = -std=c++17 -Wall


# Determine the operating system to set the output file extension
ifeq ($(OS),Windows_NT)
    EXT = .exe
else
    EXT = .out
endif

# Source files for each program
SRC1 = src/main.cpp src/newparser.cpp src/nodes.cpp src/supportFunc.cpp src/marginalizer.cpp src/conditionalizer.cpp src/VE.cpp
SRC2 = src/conditionalizerTest.cpp src/newparser.cpp src/nodes.cpp src/supportFunc.cpp src/marginalizer.cpp src/conditionalizer.cpp src/VE.cpp

# Output names
OUT1 = bin/main$(EXT)
OUT2 = bin/conditionalizerTest$(EXT)

all: $(OUT1) $(OUT2)

$(OUT1): $(SRC1)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OUT2): $(SRC2)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f main.exe main.out conditionalizerTest.exe conditionalizerTest.out
