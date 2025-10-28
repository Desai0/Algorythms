#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <numeric>

const int NUM_CITIES = 20;
const int POPULATION_SIZE = 50;
const double CROSSOVER_RATE = 0.8;
const double MUTATION_RATE = 0.05;
const int NUM_GENERATIONS = 1000;
const int ELITISM_COUNT = 2;

struct City {
    double x, y;
};

struct Individual {
    std::vector<int> chromosome;
    double route_distance;
};

std::vector<City> cities;

//вычислени расстояния
double calculateDistance(const City& city1, const City& city2) {
    return std::sqrt(std::pow(city1.x - city2.x, 2) + std::pow(city1.y - city2.y, 2));
}

//перестановка городов
std::vector<Individual> createInitialPopulation() {
    std::vector<Individual> population;
    std::vector<int> base_route(NUM_CITIES);
    std::iota(base_route.begin(), base_route.end(), 0);

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        Individual ind;
        std::random_shuffle(base_route.begin(), base_route.end());
        ind.chromosome = base_route;
        ind.route_distance = 0.0;
        population.push_back(ind);
    }
    return population;
}

//длину маршрута для каждой особи
void evaluatePopulation(std::vector<Individual>& population) {
    for (auto& ind : population) {
        double totalDistance = 0.0;
        for (size_t i = 0; i < ind.chromosome.size() - 1; ++i) {
            int city1_idx = ind.chromosome[i];
            int city2_idx = ind.chromosome[i + 1];
            totalDistance += calculateDistance(cities[city1_idx], cities[city2_idx]);
        }
        totalDistance += calculateDistance(cities[ind.chromosome.back()], cities[ind.chromosome.front()]);
        ind.route_distance = totalDistance;
    }
}

const Individual& selectParent(const std::vector<Individual>& sorted_population) {
    int parent_idx = rand() % (POPULATION_SIZE / 2);
    return sorted_population[parent_idx];
}

Individual crossover(const Individual& parent1, const Individual& parent2) {
    Individual child;
    child.chromosome.resize(NUM_CITIES, -1);

    if (static_cast<double>(rand()) / RAND_MAX < CROSSOVER_RATE) {
        int startPos = rand() % NUM_CITIES;
        int endPos = rand() % NUM_CITIES;
        if (startPos > endPos) std::swap(startPos, endPos);

        std::vector<bool> inChild(NUM_CITIES, false);

        for (int i = startPos; i <= endPos; ++i) {
            child.chromosome[i] = parent1.chromosome[i];
            inChild[parent1.chromosome[i]] = true;
        }

        int child_idx = 0;
        for (int i = 0; i < NUM_CITIES; ++i) {
            if (!inChild[parent2.chromosome[i]]) {
                while (child.chromosome[child_idx] != -1) {
                    child_idx++;
                }
                child.chromosome[child_idx] = parent2.chromosome[i];
            }
        }
    }
    else {
        child = parent1;
    }
    return child;
}

void mutate(Individual& individual) {
    if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
        int pos1 = rand() % NUM_CITIES;
        int pos2 = rand() % NUM_CITIES;
        std::swap(individual.chromosome[pos1], individual.chromosome[pos2]);
    }
}

void printBestSolution(const Individual& best) {
    std::cout << "\nЛучшее найденное решение:\n";
    std::cout << "Минимальная длина маршрута = " << best.route_distance << std::endl;
    std::cout << "Маршрут: ";
    for (int city_idx : best.chromosome) {
        std::cout << city_idx << " -> ";
    }
    std::cout << best.chromosome[0] << std::endl;
}

int main() {
    setlocale(0, "");
    srand(time(0));

    for (int i = 0; i < NUM_CITIES; ++i) {
        cities.push_back({
            static_cast<double>(rand() % 101),
            static_cast<double>(rand() % 101)
            });
    }

    std::vector<Individual> population = createInitialPopulation();

    std::cout << "--- Запуск ГА для задачи Коммивояжера ---\n";

    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        evaluatePopulation(population);

        std::sort(population.begin(), population.end(),
            [](const Individual& a, const Individual& b) {
                return a.route_distance < b.route_distance;
            });

        if (generation % 100 == 0) {
            std::cout << "Поколение " << generation
                << ", Лучшая длина: " << population[0].route_distance << std::endl;
        }

        std::vector<Individual> new_population;

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

    evaluatePopulation(population);
    std::sort(population.begin(), population.end(),
        [](const Individual& a, const Individual& b) {
            return a.route_distance < b.route_distance;
        });

    printBestSolution(population[0]);

    return 0;
}