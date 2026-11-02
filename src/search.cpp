#include "search.hpp"

int binarySearchIterative(const std::vector<Topic>& v, int id, Metrics& m) {
    int lo = 0, hi = (int)v.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        m.comparisons++;
        if (v[mid].id == id) return mid;
        if (v[mid].id < id) lo = mid + 1;  // answer must be on the right
        else hi = mid - 1;                 // answer must be on the left
    }
    return -1;
}

static int searchRec(const std::vector<Topic>& v, int id, int lo, int hi, Metrics& m) {
    if (lo > hi) return -1;  // nothing left to look at
    int mid = lo + (hi - lo) / 2;
    m.comparisons++;
    if (v[mid].id == id) return mid;
    if (v[mid].id < id) return searchRec(v, id, mid + 1, hi, m);
    return searchRec(v, id, lo, mid - 1, m);
}

int binarySearchRecursive(const std::vector<Topic>& v, int id, Metrics& m) {
    return searchRec(v, id, 0, (int)v.size() - 1, m);
}
