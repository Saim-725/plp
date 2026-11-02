#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include "loader.hpp"
#include "search.hpp"
#include "sorting.hpp"

static std::vector<Topic> randomTopics(int n, unsigned seed, int valueRange) {
    std::mt19937 rng(seed);
    std::vector<Topic> v(n);
    for (int i = 0; i < n; i++) {
        v[i].id = i;
        v[i].name = "N" + std::to_string(rng() % 50);
        v[i].hours = 1 + (int)(rng() % 5);
        v[i].value = (int)(rng() % (valueRange + 1));
        v[i].difficulty = 1 + (int)(rng() % 5);
    }
    return v;
}

static long long idSum(const std::vector<Topic>& v) {
    long long s = 0;
    for (const auto& t : v) s += t.id;
    return s;
}

static void testEdgeCases() {
    for (int algo = 0; algo < 2; algo++) {
        for (int n = 0; n <= 2; n++) {
            auto v = randomTopics(n, 7, 100);
            Metrics m;
            if (algo == 0) mergeSort(v, SortKey::Value, m); else quickSort(v, SortKey::Value, m);
            assert((int)v.size() == n);
            assert(isSorted(v, SortKey::Value));
        }
    }
}

static void testAllKeysBothAlgorithms() {
    SortKey keys[] = {SortKey::Id, SortKey::Hours, SortKey::Value,
                      SortKey::Difficulty, SortKey::ValuePerHour, SortKey::Name};
    for (SortKey key : keys) {
        auto base = randomTopics(500, 99, 10);  // small range -> many duplicates
        auto a = base, b = base;
        Metrics m1, m2;
        mergeSort(a, key, m1);
        quickSort(b, key, m2);
        assert(isSorted(a, key) && isSorted(b, key));
        assert(idSum(a) == idSum(base) && idSum(b) == idSum(base));  // nothing lost
        // merge sort must be stable: same order as std::stable_sort
        auto ref = base;
        std::stable_sort(ref.begin(), ref.end(),
                         [key](const Topic& x, const Topic& y) { return topicLess(x, y, key); });
        for (size_t i = 0; i < ref.size(); i++) assert(a[i].id == ref[i].id);
    }
}

static void testQuickSortHardInputs() {
    Metrics m;
    auto sortedIn = randomTopics(20000, 1, 1000000);
    mergeSort(sortedIn, SortKey::Value, m);
    auto reversed = sortedIn;
    std::reverse(reversed.begin(), reversed.end());
    auto equal = randomTopics(5000, 2, 0);  // every value is 0
    m.reset(); quickSort(sortedIn, SortKey::Value, m);  assert(isSorted(sortedIn, SortKey::Value));
    m.reset(); quickSort(reversed, SortKey::Value, m);  assert(isSorted(reversed, SortKey::Value));
    m.reset(); quickSort(equal, SortKey::Value, m);     assert(isSorted(equal, SortKey::Value));
}

static void testMergeSortComparisonBound() {
    int n = 4096;
    auto v = randomTopics(n, 5, 1000000);
    Metrics m;
    mergeSort(v, SortKey::Value, m);
    assert(m.comparisons <= (long long)n * 12);  // n * log2(n) = 4096 * 12
}

static void testBinarySearch() {
    std::vector<Topic> t; std::string e;
    assert(loadTopics("data/topics_small.csv", t, e));
    Metrics m;
    mergeSort(t, SortKey::Id, m);
    for (const auto& x : t) {
        Metrics a, b;
        int i1 = binarySearchIterative(t, x.id, a);
        int i2 = binarySearchRecursive(t, x.id, b);
        assert(i1 >= 0 && t[i1].id == x.id);
        assert(i1 == i2);
        assert(a.comparisons <= 5);  // 24 items -> at most 5 looks
    }
    Metrics a;
    assert(binarySearchIterative(t, 9999, a) == -1);
    assert(binarySearchRecursive(t, -5, a) == -1);
    std::vector<Topic> empty;
    assert(binarySearchIterative(empty, 1, a) == -1);
    assert(binarySearchRecursive(empty, 1, a) == -1);
}

int main() {
    testEdgeCases();
    testAllKeysBothAlgorithms();
    testQuickSortHardInputs();
    testMergeSortComparisonBound();
    testBinarySearch();
    std::cout << "All Day 2 tests passed.\n";
    return 0;
}
