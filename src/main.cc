#include "src/costfunctions/CostFunction.hh"
#include "src/costfunctions/Nonlinearity.cc"
#include <cmath>
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

int main() {

    // std::vector<uint8_t> sbox = { 153, 180, 218, 160, 120, 182, 216, 103, 93, 11, 30, 237, 82, 74, 106, 193, 241, 56, 17, 155, 116, 26, 65,
    //     32, 225, 130, 69, 14, 223, 99, 70, 121, 0, 126, 151, 19, 25, 255, 207, 254, 71, 21, 111, 192, 219, 61, 46, 145, 75, 122, 31, 154,
    //     41, 200, 50, 57, 142, 177, 188, 235, 170, 118, 58, 162, 10, 91, 181, 101, 55, 34, 179, 249, 76, 206, 83, 13, 27, 148, 159, 68, 150,
    //     85, 224, 199, 39, 44, 12, 246, 166, 98, 229, 114, 94, 194, 78, 96, 231, 147, 209, 35, 139, 86, 233, 36, 6, 5, 33, 73, 202, 123, 135,
    //     214, 227, 168, 40, 201, 244, 1, 234, 144, 191, 208, 242, 250, 161, 23, 203, 84, 66, 87, 4, 37, 110, 80, 230, 215, 42, 243, 245, 81,
    //     240, 248, 175, 190, 196, 239, 97, 212, 115, 205, 92, 141, 156, 129, 176, 72, 167, 184, 63, 171, 52, 221, 104, 228, 28, 232, 164, 38,
    //     195, 24, 9, 226, 133, 217, 113, 143, 54, 128, 53, 15, 16, 253, 90, 125, 211, 18, 112, 222, 60, 105, 172, 102, 178, 186, 109, 67, 47,
    //     146, 51, 8, 220, 173, 140, 107, 45, 252, 137, 132, 88, 152, 185, 22, 29, 157, 62, 43, 3, 189, 247, 210, 183, 49, 20, 163, 127, 134,
    //     100, 95, 174, 59, 158, 108, 79, 213, 7, 2, 204, 124, 251, 187 };


    const auto costFunction = std::make_unique<costfunctions::Nonlinearity>();

    // std::vector<uint8_t> sbox{ 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 }; // expected 4
    // std::vector<uint8_t> sbox{ 0, 0, 0, 0 }; // expected 0
    // std::vector<uint8_t> sbox = { 0, 0, 0, 1 }; // expected 1
    std::vector<uint8_t> sbox{ 0 }; // expected throw


    std::cout << "Nonlinearity of vector: [ ";
    for(auto x : sbox) {
        std::cout << static_cast<int>(x) << " ";
    }
    std::cout << "] is " << (*costFunction)(sbox) << std::endl;
}