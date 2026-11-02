#include "benchmark.hpp"
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include "metrics.hpp"
#include "sorting.hpp"

static std::vector<Topic> makeData(int n, bool alreadySorted) {
    std::mt19937 rng(12345);  // fixed seed: same data every run
    std::uniform_int_distribution<int> val(0, 1000000);
    std::vector<Topic> v(n);
    for (int i = 0; i < n; i++) {
        v[i].id = i;
        v[i].name = "T" + std::to_string(i);
        v[i].hours = 1 + i % 40;
        v[i].value = val(rng);
    }
    if (alreadySorted) { Metrics tmp; mergeSort(v, SortKey::Value, tmp); }
    return v;
}

static void runOne(const char* algo, void (*sorter)(std::vector<Topic>&, SortKey, Metrics&),
                   const std::vector<Topic>& data, int n, const char* kind) {
    std::vector<Topic> copy = data;
    Metrics m;
    Timer t;
    sorter(copy, SortKey::Value, m);
    double ms = t.elapsedMs();
    std::cout << std::setw(8) << n << std::setw(10) << kind << std::setw(12) << algo
              << std::setw(12) << std::fixed << std::setprecision(2) << ms
              << std::setw(14) << m.comparisons << std::setw(14) << m.moves
              << (isSorted(copy, SortKey::Value) ? "   OK" : "   WRONG!") << "\n";
}

void runSortBenchmark() {
    std::cout << std::setw(8) << "n" << std::setw(10) << "data" << std::setw(12) << "algorithm"
              << std::setw(12) << "time(ms)" << std::setw(14) << "comparisons"
              << std::setw(14) << "moves" << "   check\n";
    for (int n : {1000, 10000, 100000}) {
        for (bool sortedInput : {false, true}) {
            auto data = makeData(n, sortedInput);
            const char* kind = sortedInput ? "sorted" : "random";
            runOne("merge", mergeSort, data, n, kind);
            runOne("quick", quickSort, data, n, kind);
        }
    }
}
