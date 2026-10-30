#pragma once
#include <string>
#include <vector>
#include "topic.hpp"

// Loads topics from CSV: id,name,hours,value,difficulty,prereqs
// prereqs are ids separated by ';' (empty = none).
// Returns false and fills `error` on any invalid input.
bool loadTopics(const std::string& path, std::vector<Topic>& out, std::string& error);
