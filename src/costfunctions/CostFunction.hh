#pragma once

#include <vector>

namespace costfunctions {

class CostFunction {
public:
    double operator()(const std::vector<int>& input) const {
        return evaluate(input);
    }

protected:
    virtual double evaluate(const std::vector<int>& input) const = 0;
};

} // namespace costfunctions
