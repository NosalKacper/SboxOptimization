#include "src/costfunctions/DifferentialUniformity.hh"
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace costfunctions {

double DifferentialUniformity::evaluate(const std::vector<int>& input) const {
    const size_t n = input.size();

    if(n <= 1)
        throw std::invalid_argument("Input vector has to have more than one element!");
    if((n & (n - 1)) != 0)
        throw std::invalid_argument("Input vector has to have size of the power of 2!");
    if(std::ranges::any_of(input, [n](int v) { return v < 0 || static_cast<size_t>(v) >= n; }))
        throw std::invalid_argument("Input vector values have to be in range [0, size)!");

    int maxCount = 0;
    std::vector<int> row(n);

    for(size_t dx = 1; dx < n; ++dx) {
        std::ranges::fill(row, 0);
        for(size_t x = 0; x < n; ++x) {
            ++row[input[x] ^ input[x ^ dx]];
        }
        maxCount = std::max(maxCount, *std::ranges::max_element(row));
    }

    return static_cast<double>(maxCount);
}

} // namespace costfunctions
