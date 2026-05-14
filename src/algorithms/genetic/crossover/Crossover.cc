#include "src/algorithms/genetic/crossover/Crossover.hh"
#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace algorithms::genetic::crossover {

std::vector<int> pmx(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    if(lhs.size() != rhs.size()) {
        throw std::invalid_argument("Parents must have the same size");
    }

    int n = lhs.size();
    std::vector<int> child(n, -1);

    // 1. Pick crossover points
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, n - 1);

    int start = dist(gen);
    int end   = dist(gen);
    if(start > end)
        std::swap(start, end);

    // 2. Copy the segment from lhs and build the map
    // map[val] = position of val in lhs
    // Alternatively, a direct value-to-value map:
    std::vector<int> map(n, -1);
    for(int i = start; i <= end; ++i) {
        child[i] = lhs[i];
    }

    // 3. Fill the remaining positions
    for(int i = 0; i < n; ++i) {
        // Skip the crossover segment
        if(i >= start && i <= end)
            continue;

        int candidate = rhs[i];

        // Conflict resolution
        // We need to check if 'candidate' is already in the child's copied segment
        bool conflict = true;
        while(conflict) {
            conflict = false;
            for(int j = start; j <= end; ++j) {
                if(child[j] == candidate) {
                    // If candidate is already there, take the value from rhs
                    // at the same position where candidate was found in lhs
                    candidate = rhs[j];
                    conflict  = true;
                    break;
                }
            }
        }
        child[i] = candidate;
    }

    return child;
}

} // namespace algorithms::genetic::crossover