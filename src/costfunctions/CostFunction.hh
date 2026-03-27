#pragma once

#include <cstdint>
#include <vector>

namespace costfunctions {

class CostFunction {
public:
    virtual ~CostFunction() = default;

    double operator()(const std::vector<uint8_t>& input) const {
        return evaluate(input);
    }

protected:
    virtual double evaluate(const std::vector<uint8_t>& input) const = 0;
};

} // namespace costfunctions
