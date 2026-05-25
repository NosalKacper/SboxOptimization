#include "src/algorithms/genetic/Genetic.hh"
#include "src/algorithms/genetic/crossover/Crossover.hh"
#include <algorithm>
#include <iostream>
#include <numeric>

namespace algorithms::genetic {

namespace {
Chromosome createChromosome() {
    static thread_local std::mt19937 rng(std::random_device{}());

    Chromosome v(256);
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), rng);
    return v;
}
} // namespace

GeneticAlgorithm::GeneticAlgorithm(int populationSize, int iterations, std::unique_ptr<costfunctions::CostFunction> costFunction, int mutationProbability)
: mMutationProbability(mutationProbability), mIterations(iterations), fitness(std::move(costFunction)), mProgressBar(iterations) {
    mPopulation.resize(populationSize);
    std::ranges::generate(mPopulation, createChromosome);
    evaluatePopulation();
}

double GeneticAlgorithm::run() {
    for(int i = 0; i < mIterations; ++i) {
        // mProgressBar.update(i);
        nextGeneration();
        evaluatePopulation();
    }

    const auto bestNonlinearity = std::max_element(
    mPopulation.begin(), mPopulation.end(), [](const Member& a, const Member& b) { return a.fitness.value() < b.fitness.value(); });

    return (*bestNonlinearity).fitness.value();
}

const Chromosome& GeneticAlgorithm::tournamentSelect(std::mt19937& rng) {
    std::uniform_int_distribution<size_t> dist(0, mPopulation.size() - 1);

    const Member& a = mPopulation[dist(rng)];
    const Member& b = mPopulation[dist(rng)];

    return a.fitness > b.fitness ? a.chromosome : b.chromosome;
}

void GeneticAlgorithm::evaluatePopulation() {
    for(Member& member : mPopulation) {
        member.fitness = (*fitness)(member.chromosome);
    }
}

void GeneticAlgorithm::nextGeneration() {
    static thread_local std::mt19937 rng(std::random_device{}());

    Population newPopulation;
    newPopulation.reserve(mPopulation.size());

    std::uniform_int_distribution<size_t> dist(0, mPopulation.size() - 1);

    while(newPopulation.size() < mPopulation.size()) {
        const auto& p1 = tournamentSelect(rng);
        const auto& p2 = tournamentSelect(rng);

        auto child = crossover::pmx(p1, p2);

        std::uniform_int_distribution<int> dist(0, 100);
        if(dist(rng) < mMutationProbability)
            mutate(child, rng);

        newPopulation.push_back(std::move(child));
    }

    mPopulation = std::move(newPopulation);
}

void GeneticAlgorithm::mutate(Chromosome& c, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, c.size() - 1);

    int i = dist(rng);
    int j = dist(rng);
    std::swap(c[i], c[j]);
}
} // namespace algorithms::genetic