# Personalized Learning Path and Assessment Planner (DAA Project 48)

A C++17 tool that models study topics as a prerequisite graph and builds
feasible, time-bounded learning paths. Core algorithm: 0/1 Knapsack (DP),
compared against greedy and backtracking / branch and bound.

## Build and run
    make          # builds ./plp
    make test     # runs unit tests
    ./plp data/topics_small.csv
    (menu: 4 = sort, 5 = binary search, 6 = benchmark)

## CSV format
    id,name,hours,value,difficulty,prereqs     (prereqs separated by ';')

## Progress
- [x] Day 1: topic model, CSV loader with validation, topological order, CLI, tests
- [x] Day 2: metrics + timer, merge sort, quick sort, binary search (loop and recursive), sort benchmark
