#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "graph.hpp"
#include "loader.hpp"

static void writeFile(const std::string& p, const std::string& s) { std::ofstream(p) << s; }
static const std::string H = "id,name,hours,value,difficulty,prereqs\n";

static void testLoadsSampleData() {
    std::vector<Topic> t; std::string e;
    assert(loadTopics("data/topics_small.csv", t, e));
    assert(t.size() == 24);
}

static void testRejectsBadInput() {
    std::vector<Topic> t; std::string e;
    assert(!loadTopics("does_not_exist.csv", t, e));
    writeFile("/tmp/b1.csv", H + "1,A,0,5,1,\n");            assert(!loadTopics("/tmp/b1.csv", t, e));  // hours 0
    writeFile("/tmp/b2.csv", H + "1,A,3,5,9,\n");            assert(!loadTopics("/tmp/b2.csv", t, e));  // difficulty
    writeFile("/tmp/b3.csv", H + "1,A,3,5,1,\n1,B,3,5,1,\n"); assert(!loadTopics("/tmp/b3.csv", t, e)); // duplicate
    writeFile("/tmp/b4.csv", H + "1,A,3,5,1,7\n");           assert(!loadTopics("/tmp/b4.csv", t, e));  // unknown prereq
    writeFile("/tmp/b5.csv", H + "1,A,3,5,1,1\n");           assert(!loadTopics("/tmp/b5.csv", t, e));  // self prereq
    writeFile("/tmp/b6.csv", H + "1,A,x,5,1,\n");            assert(!loadTopics("/tmp/b6.csv", t, e));  // not a number
    writeFile("/tmp/b7.csv", H);                              assert(!loadTopics("/tmp/b7.csv", t, e));  // empty
}

static void testTopoOrderRespectsPrereqs() {
    std::vector<Topic> t; std::string e; std::vector<int> order;
    assert(loadTopics("data/topics_small.csv", t, e));
    assert(topologicalOrder(t, order));
    assert(order.size() == t.size());
    std::unordered_map<int, int> pos;
    for (size_t i = 0; i < order.size(); i++) pos[order[i]] = (int)i;
    for (const auto& x : t) for (int p : x.prereqs) assert(pos[p] < pos[x.id]);
}

static void testDetectsCycle() {
    std::vector<Topic> t; std::string e; std::vector<int> order;
    writeFile("/tmp/c.csv", H + "1,A,3,5,1,2\n2,B,3,5,1,1\n");
    assert(loadTopics("/tmp/c.csv", t, e));
    assert(!topologicalOrder(t, order));
}

int main() {
    testLoadsSampleData();
    testRejectsBadInput();
    testTopoOrderRespectsPrereqs();
    testDetectsCycle();
    std::cout << "All Day 1 tests passed.\n";
    return 0;
}
