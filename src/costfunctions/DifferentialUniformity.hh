#pragma once

#include "src/costfunctions/CostFunction.hh"

namespace costfunctions {

class DifferentialUniformity : public CostFunction {
protected:
    double evaluate(const std::vector<int>& input) const override;
};

} // namespace costfunctions
