#pragma once

#include "src/algorithms/genetic/crossover/Crossover.hh"
#include "src/costfunctions/CostFunction.hh"
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

namespace algorithms::genetic {

using Chromosome = std::vector<int>;
using Population = std::vector<Chromosome>;

Chromosome createChromosome() {
    static thread_local std::mt19937 rng(std::random_device{}());

    Chromosome v(256);
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), rng);
    return v;
}

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int populationSize, int iterations, std::unique_ptr<costfunctions::CostFunction> costFunction)
    : mIterations(iterations), fitness(std::move(costFunction)) {
        mPopulation.resize(populationSize);
        std::ranges::generate(mPopulation, createChromosome);
    }

    void run() {
        for(int i = 0; i < mIterations; ++i) {
            next_generation();
            std::cout << i << std::endl;
        }


        const auto bestNonlinearity = std::max_element(
        mPopulation.begin(), mPopulation.end(), [this](const Chromosome& a, const Chromosome& b) { return (*fitness)(a) < (*fitness)(b); });

        std::cout << "Best nonlinearity is: " << (*fitness)(*bestNonlinearity) << "\n";
    }

    const Chromosome& tournament_select(std::mt19937& rng) {
        std::uniform_int_distribution<size_t> dist(0, mPopulation.size() - 1);

        const Chromosome& a = mPopulation[dist(rng)];
        const Chromosome& b = mPopulation[dist(rng)];

        return (*fitness)(a) > (*fitness)(b) ? a : b;
    }

    void next_generation() {
        static thread_local std::mt19937 rng(std::random_device{}());

        Population newPopulation;
        newPopulation.reserve(mPopulation.size());

        std::uniform_int_distribution<size_t> dist(0, mPopulation.size() - 1);

        while(newPopulation.size() < mPopulation.size()) {
            const auto& p1 = tournament_select(rng);
            const auto& p2 = tournament_select(rng);

            auto child = crossover::pmx(p1, p2);
            mutate(child, rng);

            newPopulation.push_back(std::move(child));
        }

        mPopulation = std::move(newPopulation);
    }

    void mutate(Chromosome& c, std::mt19937& rng) {
        std::uniform_int_distribution<int> dist(0, c.size() - 1);

        int i = dist(rng);
        int j = dist(rng);
        std::swap(c[i], c[j]);
    }

private:
    int mIterations;
    std::unique_ptr<costfunctions::CostFunction> fitness;

    Population mPopulation;
};

} // namespace algorithms::genetic