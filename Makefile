CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude
LIB_SRC  := src/loader.cpp src/graph.cpp src/sorting.cpp src/search.cpp src/benchmark.cpp

all: plp

plp: $(LIB_SRC) src/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(LIB_SRC) tests/test_basic.cpp tests/test_day2.cpp
	$(CXX) $(CXXFLAGS) $(LIB_SRC) tests/test_basic.cpp -o test_basic && ./test_basic
	$(CXX) $(CXXFLAGS) $(LIB_SRC) tests/test_day2.cpp -o test_day2 && ./test_day2

clean:
	rm -f plp test_basic test_day2

.PHONY: all test clean
