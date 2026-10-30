#include <iostream>
#include <string>
#include <unordered_map>
#include "graph.hpp"
#include "loader.hpp"

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

int main(int argc, char** argv) {
    std::vector<Topic> topics;
    std::string path = argc > 1 ? argv[1] : "data/topics_small.csv";
    std::string err;
    if (loadTopics(path, topics, err)) std::cout << "Loaded " << topics.size() << " topics from " << path << "\n";
    else std::cout << "Load failed: " << err << "\n";

    while (true) {
        std::cout << "\n=== Personalized Learning Path Planner ===\n"
                     "1. Load topics from CSV\n2. List topics\n3. Show valid study order\n0. Exit\n> ";
        int c;
        if (!(std::cin >> c)) break;
        if (c == 0) break;
        if (c == 1) {
            std::cout << "CSV path: ";
            std::cin >> path;
            if (loadTopics(path, topics, err)) std::cout << "Loaded " << topics.size() << " topics.\n";
            else std::cout << "Load failed: " << err << "\n";
        } else if (c == 2) listTopics(topics);
        else if (c == 3) showOrder(topics);
        else std::cout << "Invalid choice.\n";
    }
    return 0;
}
