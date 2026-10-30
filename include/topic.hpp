#pragma once
#include <string>
#include <vector>

// One study topic in the learning-path graph.
struct Topic {
    int id = 0;
    std::string name;
    int hours = 0;       // study time needed (knapsack weight)
    int value = 0;       // mastery value gained (knapsack value)
    int difficulty = 1;  // 1 (easy) .. 5 (hard)
    std::vector<int> prereqs;  // ids of topics that must be done first
};
