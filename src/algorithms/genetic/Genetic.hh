#pragma once

#include "src/costfunctions/CostFunction.hh"
#include "src/utils/ProgressBar.hh"
#include <memory>
#include <optional>
#include <random>
#include <vector>

namespace algorithms::genetic {

using Chromosome = std::vector<int>;

struct Member {

    Member(Chromosome chromosome) : chromosome(chromosome), fitness(std::nullopt) {
    }

    Member() = default;

    Chromosome chromosome;
    std::optional<double> fitness;
};

using Population = std::vector<Member>;


class GeneticAlgorithm {
public:
    GeneticAlgorithm(int populationSize, int iterations, std::unique_ptr<costfunctions::CostFunction> costFunction);

    double run();

private:
    const Chromosome& tournamentSelect(std::mt19937& rng);
    void nextGeneration();
    void mutate(Chromosome& c, std::mt19937& rng);
    void evaluatePopulation();

    int mMutationProbability;
    int mIterations;
    std::unique_ptr<costfunctions::CostFunction> fitness;
    Population mPopulation;
    ProgressBar mProgressBar;
};

} // namespace algorithms::genetic