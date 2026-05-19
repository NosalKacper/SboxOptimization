#include "src/costfunctions/Nonlinearity.hh"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace costfunctions {

namespace {

bool isPowerOfTwo(const size_t input) {
    return input != (1u << static_cast<int>(std::log2(input) + 0.5));
}

std::vector<int> fastWalshHadmardTransform(std::vector<int> data) {
    int n = data.size();

    for(int len = 1; len < n; len <<= 1) {
        for(int i = 0; i < n; i += 2 * len) {
            for(int j = 0; j < len; j++) {
                int u             = data[i + j];
                int v             = data[i + j + len];
                data[i + j]       = u + v;
                data[i + j + len] = u - v;
            }
        }
    }
    return data;
}

int computeNonlinearityOfBinaryFunction(const std::vector<int>& input) {
    int n = static_cast<int>(std::log2(input.size()));

    std::vector<int> transformInput(input.size());
    std::transform(input.begin(), input.end(), transformInput.begin(), [](int val) { return (val == 0) ? 1 : -1; });

    const std::vector<int> transformProduct = fastWalshHadmardTransform(transformInput);

    int maxAbsValue = 0;
    for(int val : transformProduct) {
        maxAbsValue = std::max(maxAbsValue, std::abs(val));
    }

    return (1 << (n - 1)) - (maxAbsValue / 2);
}

} // namespace

double Nonlinearity::evaluate(const std::vector<int>& input) const {
    if(input.size() <= 1)
        throw std::invalid_argument("Input vector has to have more than one element!");
    if(isPowerOfTwo(input.size()))
        throw std::invalid_argument("Input vector has to have size of the power of 2!");


    int minimumNonlinearity = INT16_MAX;

    for(int mask = 1; mask <= 255; ++mask) {
        std::vector<int> combinedFunction(input.size());

        for(size_t i = 0; i < input.size(); ++i) {

            int combinedBit = 0;
            int maskedValue = input[i] & mask;

            combinedBit = __builtin_popcount(maskedValue) % 2;

            combinedFunction[i] = combinedBit;
        }

        int currentNL = computeNonlinearityOfBinaryFunction(combinedFunction);

        if(currentNL < minimumNonlinearity) {
            minimumNonlinearity = currentNL;
        }
    }

    return (double)minimumNonlinearity;
}
} // namespace costfunctions