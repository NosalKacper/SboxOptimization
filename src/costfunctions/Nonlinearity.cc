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

int computeNonlinearityOfBinaryFunction(const std::vector<uint8_t>& input) {
    int n = static_cast<int>(std::log2(input.size()));

    std::vector<int> transformInput(input.size());
    std::transform(input.begin(), input.end(), transformInput.begin(), [](uint8_t val) { return (val == 0) ? 1 : -1; });

    const std::vector<int> transformProduct = fastWalshHadmardTransform(transformInput);

    int maxAbsValue = 0;
    for(int val : transformProduct) {
        maxAbsValue = std::max(maxAbsValue, std::abs(val));
    }

    return (1 << (n - 1)) - (maxAbsValue / 2);
}

} // namespace

double Nonlinearity::evaluate(const std::vector<uint8_t>& input) const {
    if(input.size() <= 1)
        throw std::invalid_argument("Input vector has to have more than one element!");

    int maximumNonlinearity = 0;
    for(size_t bit = 0; bit <= 7; ++bit) {

        std::vector<uint8_t> binaryFunction(input.size());

        std::transform(input.begin(), input.end(), binaryFunction.begin(), [bit](auto value) { return (value >> bit) & 1; });

        int tmp             = computeNonlinearityOfBinaryFunction(binaryFunction);
        maximumNonlinearity = std::max(tmp, maximumNonlinearity);
    }

    return maximumNonlinearity;
}
} // namespace costfunctions