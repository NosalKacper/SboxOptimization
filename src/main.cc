#include "src/algorithms/genetic/Genetic.hh"
#include "src/algorithms/genetic/crossover/Crossover.hh"
#include "src/costfunctions/Nonlinearity.hh"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

void printUsage() {
    std::cerr << "Usage: ./SboxOptimization -p <population_size> -i <iteration_count>\n"
              << "\nOptions:\n"
              << "  -p <population_size>   Number of individuals in the population\n"
              << "  -i <iteration_count>  Number of optimization iterations\n"
              << "\nExample:\n"
              << "  ./SboxOptimization -p 100 -i 1000\n";
}

int main(int argc, char* argv[]) {

    if(argc <= 1) {
        printUsage();
        return -1;
    }

    int populationSize      = 0;
    int iterations          = 0;
    int mutationProbability = 0;

    int opt;
    while((opt = getopt(argc, argv, "p:i:m:")) != -1) {
        switch(opt) {
        case 'p': populationSize = std::stoi(optarg); break;
        case 'i': iterations = std::stoi(optarg); break;
        case 'm': mutationProbability = std::stoi(optarg); break;
        default: printUsage(); return 1;
        }
    }

    algorithms::genetic::GeneticAlgorithm genetic(populationSize, iterations, std::make_unique<costfunctions::Nonlinearity>(), mutationProbability);

    std::cout << std::fixed << std::setprecision(10) << genetic.run() << std::endl;

    return 0;
}
