#include "loader.hpp"
#include <fstream>
#include <sstream>
#include <unordered_set>

static bool toInt(const std::string& s, int& v) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '-') i = 1;
    if (i == s.size()) return false;
    for (size_t k = i; k < s.size(); k++)
        if (s[k] < '0' || s[k] > '9') return false;
    if (s.size() > 9) return false;  // avoid overflow
    v = std::stoi(s);
    return true;
}

static std::vector<std::string> split(const std::string& line, char d) {
    std::vector<std::string> parts;
    std::string cur;
    for (char c : line) {
        if (c == d) { parts.push_back(cur); cur.clear(); }
        else if (c != '\r') cur += c;
    }
    parts.push_back(cur);
    return parts;
}

bool loadTopics(const std::string& path, std::vector<Topic>& out, std::string& error) {
    out.clear();
    std::ifstream in(path);
    if (!in) { error = "cannot open file: " + path; return false; }

    std::string line;
    int lineNo = 0;
    std::unordered_set<int> ids;
    while (std::getline(in, line)) {
        lineNo++;
        if (lineNo == 1 || line.empty() || line == "\r") continue;  // header / blanks
        auto f = split(line, ',');
        std::ostringstream where;
        where << "line " << lineNo << ": ";
        if (f.size() != 6) { error = where.str() + "expected 6 fields"; return false; }

        Topic t;
        if (!toInt(f[0], t.id) || t.id < 0) { error = where.str() + "bad id"; return false; }
        t.name = f[1];
        if (t.name.empty()) { error = where.str() + "empty name"; return false; }
        if (!toInt(f[2], t.hours) || t.hours <= 0) { error = where.str() + "hours must be > 0"; return false; }
        if (!toInt(f[3], t.value) || t.value < 0) { error = where.str() + "value must be >= 0"; return false; }
        if (!toInt(f[4], t.difficulty) || t.difficulty < 1 || t.difficulty > 5) {
            error = where.str() + "difficulty must be 1..5"; return false;
        }
        if (!f[5].empty()) {
            for (const auto& p : split(f[5], ';')) {
                int pid;
                if (!toInt(p, pid)) { error = where.str() + "bad prerequisite id"; return false; }
                if (pid == t.id) { error = where.str() + "topic cannot require itself"; return false; }
                t.prereqs.push_back(pid);
            }
        }
        if (!ids.insert(t.id).second) { error = where.str() + "duplicate id"; return false; }
        out.push_back(t);
    }
    // every prerequisite must refer to an existing topic
    for (const auto& t : out)
        for (int p : t.prereqs)
            if (!ids.count(p)) {
                error = "topic " + std::to_string(t.id) + " has unknown prerequisite " + std::to_string(p);
                return false;
            }
    if (out.empty()) { error = "no topics found"; return false; }
    return true;
}
