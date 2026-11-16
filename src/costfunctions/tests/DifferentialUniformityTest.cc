#include "src/costfunctions/DifferentialUniformity.hh"
#include "src/costfunctions/CostFunction.hh"
#include "gtest/gtest.h"
#include <algorithm>
#include <memory>
#include <numeric>

namespace costfunctions::tests {

using TestParam = std::tuple<std::vector<int>, double, bool>;

namespace {

// Multiplication in GF(2^8) modulo the AES polynomial x^8 + x^4 + x^3 + x + 1.
int gfMul(int a, int b) {
    int r = 0;
    while(b) {
        if(b & 1)
            r ^= a;
        a <<= 1;
        if(a & 0x100)
            a ^= 0x11b;
        b >>= 1;
    }
    return r;
}

int rotl8(int v, int s) {
    return ((v << s) | (v >> (8 - s))) & 0xff;
}

// AES S-box: multiplicative inverse in GF(2^8) followed by the affine transform.
std::vector<int> aesSbox() {
    std::vector<int> sbox(256);
    for(int x = 0; x < 256; ++x) {
        int inv = 0;
        for(int y = 1; y < 256 && x != 0; ++y) {
            if(gfMul(x, y) == 1) {
                inv = y;
                break;
            }
        }
        sbox[x] = inv ^ rotl8(inv, 1) ^ rotl8(inv, 2) ^ rotl8(inv, 3) ^ rotl8(inv, 4) ^ 0x63;
    }
    return sbox;
}

std::vector<int> identity(int n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

} // namespace

class DifferentialUniformityTest : public testing::TestWithParam<TestParam> {
public:
    std::unique_ptr<costfunctions::CostFunction> mUnderTest = std::make_unique<costfunctions::DifferentialUniformity>();
};

TEST_F(DifferentialUniformityTest, aesSboxShouldHaveUniformityFour) {
    const auto sbox = aesSbox();
    ASSERT_EQ(sbox[0x00], 0x63);
    ASSERT_EQ(sbox[0x53], 0xed);

    EXPECT_EQ((*mUnderTest)(sbox), 4.0);
}

TEST_F(DifferentialUniformityTest, xorOnOutputShouldNotImpactUniformity) {
    const auto sbox = aesSbox();
    std::vector<int> shifted(sbox.size());
    std::ranges::transform(sbox, shifted.begin(), [](int v) { return v ^ 0x5a; });

    EXPECT_EQ((*mUnderTest)(sbox), (*mUnderTest)(shifted));
}

TEST_F(DifferentialUniformityTest, xorOnInputShouldNotImpactUniformity) {
    const auto sbox = aesSbox();
    std::vector<int> shifted(sbox.size());
    for(size_t x = 0; x < sbox.size(); ++x)
        shifted[x] = sbox[x ^ 0xa7];

    EXPECT_EQ((*mUnderTest)(sbox), (*mUnderTest)(shifted));
}

TEST_F(DifferentialUniformityTest, inverseShouldHaveSameUniformity) {
    const auto sbox = aesSbox();
    std::vector<int> inverse(sbox.size());
    for(size_t x = 0; x < sbox.size(); ++x)
        inverse[sbox[x]] = static_cast<int>(x);

    EXPECT_EQ((*mUnderTest)(sbox), (*mUnderTest)(inverse));
}

TEST_P(DifferentialUniformityTest, DifferentialUniformityTest) {
    const auto& [input, expected, shouldThrow] = GetParam();

    if(shouldThrow) {
        EXPECT_THROW((*mUnderTest)(input), std::exception);
        return;
    }

    EXPECT_EQ((*mUnderTest)(input), expected);
}

INSTANTIATE_TEST_SUITE_P(DifferentialUniformityCases,
DifferentialUniformityTest,
::testing::Values(
// Linear maps are the worst case: uniformity equals the size.
TestParam{ identity(4), 4.0, false },
TestParam{ identity(256), 256.0, false },
// x^3 over GF(2^3) is APN: uniformity 2, the best possible.
TestParam{ std::vector<int>{ 0, 1, 3, 4, 5, 6, 7, 2 }, 2.0, false },
// Constant function: every difference maps to 0.
TestParam{ std::vector<int>{ 0, 0, 0, 0 }, 4.0, false },
TestParam{ std::vector<int>{ 1, 2, 3, 0, 1 }, 0.0, true },
TestParam{ std::vector<int>{ 0 }, 0.0, true },
TestParam{ std::vector<int>{ 0, 1, 2, 4 }, 0.0, true },
TestParam{ std::vector<int>{ 0, -1, 2, 3 }, 0.0, true }));

} // namespace costfunctions::tests
