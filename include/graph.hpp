#pragma once
#include <vector>
#include "topic.hpp"

// Kahn's algorithm. Fills `order` with topic ids so every prerequisite
// comes before the topic that needs it. Returns false if a cycle exists.
// Time O(V + E), space O(V + E).
bool topologicalOrder(const std::vector<Topic>& topics, std::vector<int>& order);
