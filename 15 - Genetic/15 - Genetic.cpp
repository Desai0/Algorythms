#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int NUM_VARIABLES = 10;
const double MIN_VAL = -5.12;
const double MAX_VAL = 5.11;
const int POPULATION_SIZE = 20;
const double CROSSOVER_RATE = 0.7;
const double MUTATION_RATE = 0.05;
const int NUM_GENERATIONS = 200;
const int ELITISM_COUNT = 2; // Сколько лучших выживает

struct Individual {
    std::vector<double> chromosome;
    double z_value;
};

double randomDouble(double min, double max) {
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}


std::vector<Individual> createInitialPopulation() {
    std::vector<Individual> population;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        Individual ind;
        for (int j = 0; j < NUM_VARIABLES; ++j) {
            ind.chromosome.push_back(randomDouble(MIN_VAL, MAX_VAL));
        }
        population.push_back(ind);
    }
    return population;
}

// знач функ z
void evaluatePopulation(std::vector<Individual>& population) {
    for (auto& ind : population) {
        double sum_of_squares = 0.0;
        for (double gene : ind.chromosome) {
            sum_of_squares += gene * gene;
        }
        ind.z_value = sum_of_squares;
    }
}


const Individual& selectParent(const std::vector<Individual>& sorted_population) {
    int parent_idx = rand() % (POPULATION_SIZE / 2);
    return sorted_population[parent_idx];
}


Individual crossover(const Individual& parent1, const Individual& parent2) {
    Individual child = parent1;

    if (randomDouble(0, 1) < CROSSOVER_RATE) {
        int p1 = 1 + rand() % (NUM_VARIABLES - 2);
        int p2 = 1 + rand() % (NUM_VARIABLES - 2);
        if (p1 > p2) std::swap(p1, p2);

        for (int i = p1; i < p2; ++i) {
            child.chromosome[i] = parent2.chromosome[i];
        }
    }
    return child;
}

void mutate(Individual& individual) {
    for (int i = 0; i < NUM_VARIABLES; ++i) {
        if (randomDouble(0, 1) < MUTATION_RATE) {
            individual.chromosome[i] = randomDouble(MIN_VAL, MAX_VAL);
        }
    }
}


void printBestSolution(const Individual& best) {
    std::cout << "\n--- Алгоритм завершен ---\n";
    std::cout << "Лучшее найденное решение:\n";
    std::cout << "Минимальное Z = " << best.z_value << std::endl;
    std::cout << "При значениях x_i:\n";
    for (int i = 0; i < NUM_VARIABLES; ++i) {
        std::cout << "x" << (i + 1) << " = " << best.chromosome[i] << std::endl;
    }
}



int main() {
    setlocale(0, "");

    srand(time(0));

    std::vector<Individual> population = createInitialPopulation();

    std::cout << "--- Запуск ГА с разделением на функции ---\n";

    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {

        evaluatePopulation(population);

        // выбор лучших
        std::sort(population.begin(), population.end(),
            [](const Individual& a, const Individual& b) {
                return a.z_value < b.z_value;
            });

        std::cout << "Поколение " << generation + 1
            << ", Лучшее Z: " << population[0].z_value << std::endl;

        std::vector<Individual> new_population;

        // лучшие выживают
        for (int i = 0; i < ELITISM_COUNT; ++i) {
            new_population.push_back(population[i]);
        }

        while (new_population.size() < POPULATION_SIZE) {
            const Individual& parent1 = selectParent(population);
            const Individual& parent2 = selectParent(population);

            Individual child = crossover(parent1, parent2);
            mutate(child);

            new_population.push_back(child);
        }

        population = new_population;
    }

    printBestSolution(population[0]);

    return 0;
}