#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include "benchmark.hpp"
#include "graph.hpp"
#include "loader.hpp"
#include "search.hpp"
#include "sorting.hpp"

// Reads a whole number from the keyboard. Returns false only if input ended.
static bool readInt(const std::string& prompt, int& out) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> out) return true;
        if (std::cin.eof()) return false;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please type a number.\n";
    }
}

static void listTopics(const std::vector<Topic>& ts) {
    std::cout << "ID\tName\tHrs\tVal\tDiff\tPrereqs\n";
    for (const auto& t : ts) {
        std::cout << t.id << "\t" << t.name << "\t" << t.hours << "\t" << t.value
                  << "\t" << t.difficulty << "\t";
        for (size_t i = 0; i < t.prereqs.size(); i++)
            std::cout << (i ? ";" : "") << t.prereqs[i];
        std::cout << "\n";
    }
}

static void showOrder(const std::vector<Topic>& ts) {
    std::vector<int> order;
    if (!topologicalOrder(ts, order)) {
        std::cout << "Cycle detected in prerequisites - no valid study order.\n";
        return;
    }
    std::unordered_map<int, const Topic*> byId;
    for (const auto& t : ts) byId[t.id] = &t;
    std::cout << "Valid study order:\n";
    int step = 1;
    for (int id : order) std::cout << step++ << ". " << byId[id]->name << " (id " << id << ")\n";
}

static void sortMenu(const std::vector<Topic>& ts) {
    int algo, k;
    if (!readInt("Algorithm: 1) merge sort  2) quick sort > ", algo)) return;
    if (!readInt("Sort by: 1) id 2) hours 3) value 4) difficulty 5) value-per-hour 6) name > ", k)) return;
    if ((algo != 1 && algo != 2) || k < 1 || k > 6) { std::cout << "Invalid choice.\n"; return; }
    SortKey keys[] = {SortKey::Id, SortKey::Hours, SortKey::Value,
                      SortKey::Difficulty, SortKey::ValuePerHour, SortKey::Name};
    SortKey key = keys[k - 1];

    std::vector<Topic> copy = ts;  // sort a copy, keep the original untouched
    Metrics m;
    Timer t;
    if (algo == 1) mergeSort(copy, key, m); else quickSort(copy, key, m);
    double ms = t.elapsedMs();

    listTopics(copy);
    std::cout << (algo == 1 ? "Merge sort" : "Quick sort") << " by " << sortKeyName(key)
              << ": " << m.comparisons << " comparisons, " << m.moves << " moves, "
              << ms << " ms\n";
}

static void findMenu(const std::vector<Topic>& ts) {
    int id;
    if (!readInt("Topic id to find > ", id)) return;
    std::vector<Topic> copy = ts;
    Metrics sm;
    mergeSort(copy, SortKey::Id, sm);  // binary search needs a sorted list

    Metrics a, b;
    int i1 = binarySearchIterative(copy, id, a);
    int i2 = binarySearchRecursive(copy, id, b);
    if (i1 < 0) { std::cout << "No topic with id " << id << ".\n"; return; }
    std::cout << "Found: " << copy[i1].name << " (" << copy[i1].hours << " hrs, value "
              << copy[i1].value << ")\n"
              << "Loop version looked at " << a.comparisons << " items; "
              << "recursive version looked at " << b.comparisons << " items "
              << "(out of " << copy.size() << ").\n";
    (void)i2;
}

int main(int argc, char** argv) {
    std::vector<Topic> topics;
    std::string path = argc > 1 ? argv[1] : "data/topics_small.csv";
    std::string err;
    if (loadTopics(path, topics, err)) std::cout << "Loaded " << topics.size() << " topics from " << path << "\n";
    else std::cout << "Load failed: " << err << "\n";

    while (true) {
        std::cout << "\n=== Personalized Learning Path Planner ===\n"
                     "1. Load topics from CSV\n2. List topics\n3. Show valid study order\n"
                     "4. Sort topics\n5. Find a topic by id (binary search)\n"
                     "6. Benchmark sorting algorithms\n0. Exit\n";
        int c;
        if (!readInt("> ", c)) break;
        if (c == 0) break;
        bool needData = (c >= 2 && c <= 5);
        if (needData && topics.empty()) { std::cout << "Load topics first (option 1).\n"; continue; }
        if (c == 1) {
            std::cout << "CSV path: ";
            std::cin >> path;
            if (loadTopics(path, topics, err)) std::cout << "Loaded " << topics.size() << " topics.\n";
            else std::cout << "Load failed: " << err << "\n";
        } else if (c == 2) listTopics(topics);
        else if (c == 3) showOrder(topics);
        else if (c == 4) sortMenu(topics);
        else if (c == 5) findMenu(topics);
        else if (c == 6) runSortBenchmark();
        else std::cout << "Invalid choice.\n";
    }
    return 0;
}
