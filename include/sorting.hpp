#pragma once
#include <vector>
#include "metrics.hpp"
#include "topic.hpp"

// What we sort the topics by.
enum class SortKey { Id, Hours, Value, Difficulty, ValuePerHour, Name };

const char* sortKeyName(SortKey k);

// Should topic a come before topic b when sorting by `key`?
bool topicLess(const Topic& a, const Topic& b, SortKey key);

// True if the list is already in order.
bool isSorted(const std::vector<Topic>& v, SortKey key);

// Merge sort: split in half, sort each half, merge. Always O(n log n). Stable.
void mergeSort(std::vector<Topic>& v, SortKey key, Metrics& m);

// Quick sort: pick a pivot, put smaller items left and bigger right, repeat.
// Average O(n log n). Uses median-of-three pivot to avoid slow sorted inputs.
void quickSort(std::vector<Topic>& v, SortKey key, Metrics& m);
