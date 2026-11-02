#pragma once
#include <vector>
#include "metrics.hpp"
#include "topic.hpp"

// Binary search for a topic id. The list MUST already be sorted by id.
// Look at the middle, throw away the wrong half, repeat. O(log n).
// Both return the position of the topic, or -1 if it is not there.
// m.comparisons counts how many middle items were looked at.
int binarySearchIterative(const std::vector<Topic>& v, int id, Metrics& m);  // uses a loop
int binarySearchRecursive(const std::vector<Topic>& v, int id, Metrics& m);  // calls itself
