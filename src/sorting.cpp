#include "sorting.hpp"
#include <utility>

const char* sortKeyName(SortKey k) {
    switch (k) {
        case SortKey::Id: return "id";
        case SortKey::Hours: return "hours";
        case SortKey::Value: return "value";
        case SortKey::Difficulty: return "difficulty";
        case SortKey::ValuePerHour: return "value-per-hour";
        case SortKey::Name: return "name";
    }
    return "?";
}

bool topicLess(const Topic& a, const Topic& b, SortKey key) {
    switch (key) {
        case SortKey::Id: return a.id < b.id;
        case SortKey::Hours: return a.hours < b.hours;
        case SortKey::Value: return a.value < b.value;
        case SortKey::Difficulty: return a.difficulty < b.difficulty;
        // value/hours compared by cross-multiplying, so no decimals are needed
        case SortKey::ValuePerHour:
            return (long long)a.value * b.hours < (long long)b.value * a.hours;
        case SortKey::Name: return a.name < b.name;
    }
    return false;
}

bool isSorted(const std::vector<Topic>& v, SortKey key) {
    for (size_t i = 1; i < v.size(); i++)
        if (topicLess(v[i], v[i - 1], key)) return false;
    return true;
}

// ---------------- Merge sort ----------------
// Sorts the half-open range [lo, hi).
static void mergeRec(std::vector<Topic>& v, std::vector<Topic>& tmp, int lo, int hi,
                     SortKey key, Metrics& m) {
    if (hi - lo < 2) return;  // 0 or 1 item is already sorted
    int mid = lo + (hi - lo) / 2;
    mergeRec(v, tmp, lo, mid, key, m);
    mergeRec(v, tmp, mid, hi, key, m);

    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        m.comparisons++;
        // take from the right only if it is strictly smaller (keeps it stable)
        if (topicLess(v[j], v[i], key)) tmp[k++] = v[j++];
        else tmp[k++] = v[i++];
        m.moves++;
    }
    while (i < mid) { tmp[k++] = v[i++]; m.moves++; }
    while (j < hi) { tmp[k++] = v[j++]; m.moves++; }
    for (int x = lo; x < hi; x++) v[x] = tmp[x];
    m.moves += hi - lo;
}

void mergeSort(std::vector<Topic>& v, SortKey key, Metrics& m) {
    std::vector<Topic> tmp(v.size());
    mergeRec(v, tmp, 0, (int)v.size(), key, m);
}

// ---------------- Quick sort ----------------
static void swapAt(std::vector<Topic>& v, int i, int j, Metrics& m) {
    if (i == j) return;
    std::swap(v[i], v[j]);
    m.moves++;
}

// Index of the median of v[a], v[b], v[c].
static int medianOfThree(const std::vector<Topic>& v, int a, int b, int c,
                         SortKey key, Metrics& m) {
    m.comparisons += 3;
    bool ab = topicLess(v[a], v[b], key);
    bool bc = topicLess(v[b], v[c], key);
    bool ac = topicLess(v[a], v[c], key);
    if (ab == bc) return b;
    return (ab == ac) ? c : a;
}

// Puts the pivot in its final place; smaller items end up on its left.
static int partition(std::vector<Topic>& v, int lo, int hi, SortKey key, Metrics& m) {
    int mid = lo + (hi - lo) / 2;
    int med = medianOfThree(v, lo, mid, hi, key, m);
    swapAt(v, med, hi, m);  // pivot now sits at the end
    int i = lo;
    for (int j = lo; j < hi; j++) {
        m.comparisons++;
        if (topicLess(v[j], v[hi], key)) { swapAt(v, i, j, m); i++; }
    }
    swapAt(v, i, hi, m);
    return i;
}

// Sorts the closed range [lo, hi]. Recursing on the smaller part and looping
// on the bigger part keeps the call stack only O(log n) deep.
static void quickRec(std::vector<Topic>& v, int lo, int hi, SortKey key, Metrics& m) {
    while (lo < hi) {
        int p = partition(v, lo, hi, key, m);
        if (p - lo < hi - p) { quickRec(v, lo, p - 1, key, m); lo = p + 1; }
        else { quickRec(v, p + 1, hi, key, m); hi = p - 1; }
    }
}

void quickSort(std::vector<Topic>& v, SortKey key, Metrics& m) {
    quickRec(v, 0, (int)v.size() - 1, key, m);
}
