CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude
LIB_SRC  := src/loader.cpp src/graph.cpp

all: plp

plp: $(LIB_SRC) src/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(LIB_SRC) tests/test_basic.cpp
	$(CXX) $(CXXFLAGS) $^ -o test_basic && ./test_basic

clean:
	rm -f plp test_basic

.PHONY: all test clean
