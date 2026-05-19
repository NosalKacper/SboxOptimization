#include "src/costfunctions/Nonlinearity.hh"
#include "src/costfunctions/CostFunction.hh"
#include "gtest/gtest.h"
#include <memory>

namespace costfunctions::tests {

using TestParam = std::tuple<std::vector<int>, double, bool>;

class NonlinearityTest : public testing::TestWithParam<TestParam> {
public:
    std::unique_ptr<costfunctions::CostFunction> mUnderTest = std::make_unique<costfunctions::Nonlinearity>();
};

TEST_F(NonlinearityTest, multiplicationShouldNotImpactNonlinearity) {

    std::vector<int> lhs{ 3, 8, 7, 3, 13, 25, 16, 6 };
    std::vector<int> rhs{ 30, 80, 70, 30, 130, 250, 160, 60 };

    EXPECT_EQ((*mUnderTest)(lhs), (*mUnderTest)(rhs));
}

TEST_F(NonlinearityTest, reverseOrderShouldNotImpactNonlinearity) {

    std::vector<int> lhs{ 3, 8, 7, 3, 13, 25, 16, 6 };
    std::vector<int> rhs{ 6, 16, 25, 13, 3, 7, 8, 3 };

    EXPECT_EQ((*mUnderTest)(lhs), (*mUnderTest)(rhs));
}

TEST_P(NonlinearityTest, NonlinearityTest) {
    const auto& [input, expected, shouldThrow] = GetParam();

    if(shouldThrow) {
        EXPECT_THROW((*mUnderTest)(input), std::exception);
        return;
    }

    const auto result = (*mUnderTest)(input);
    EXPECT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(NonlinearityCases,
NonlinearityTest,
::testing::Values(TestParam{ std::vector<int>{ 0, 0, 0, 0 }, 0.0, false },
TestParam{ std::vector<int>{ 1, 2, 3, 4 }, 0.0, false },
TestParam{ std::vector<int>{ 1, 2, 3, 4, 5 }, 0.0, true },
TestParam{ std::vector<int>{ 0 }, 0.0, true }));

} // namespace costfunctions::tests