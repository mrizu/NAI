#include <iostream>
#include <vector>
#include <random>

using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;
std::random_device rd;
std::mt19937 mt_generator(rd());

auto genetic_algorithm = [](
        const population_t& initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation, auto mutation) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0, 1.0);
    population_t population = initial_population;
    vector<double> population_fit = fitness(population);

    while (!term_condition(population, population_fit)) {
        vector<int> parents_indexes = selection(population_fit);
        population_t new_population;

        for (int i = 0; i < parents_indexes.size(); i += 2) {
            population_t parents = {population[i], population[i + 1]};
            if (uniform(mt_generator) < p_crossover) {
                population_t offspring = crossover(parents);
                for (const chromosome_t& chromosome: offspring) new_population.push_back(chromosome);
            }
        }
        for (auto &chromosome: new_population) {
            chromosome = mutation(chromosome, p_mutation);
        }

        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};

std::pair<double, double> decode(chromosome_t chromosome) {
    double x = 0.0;
    double y = 0.0;
    double base_x = 1.0;
    double base_y = 1.0;
    int half_c = chromosome.size() / 2;

    for (int i = 0; i < half_c; i++) {
        if (chromosome[i] == 1) {
            x += base_x;
        }
        base_x = base_x * 2;
    }

    for (int i = half_c; i < chromosome.size(); i++) {
        if (chromosome[i] == 1) {
            y += base_y;
        }
        base_y = base_y * 2;
    }
    x = x / pow(2.0, (half_c - 4)) - 8;
    y = y / pow(2.0, (half_c - 4)) - 8;

    return {x, y};
}

population_t population_generator(int size) {
    population_t population;
    for (int i = 0; i < size; i++) {
        chromosome_t chromosome;
        for (int j = 0; j < 100; j++) {
            std::uniform_int_distribution<int> byte(0, 1);
            chromosome.push_back(byte(mt_generator));
        }
        population.push_back(chromosome);
    }
    return population;
}

std::vector<int> selection(std::vector<double> fitnesses) {
    std::vector<int> indexes;
    std::vector<int> roulette_arr;
    int size = 100 + (23366 % 10) * 2;
    for (int i = 0; i < size; i++){
        for (int j = 0; j<fitnesses[i]*100; j++){
            roulette_arr.push_back(i);
        }
    }

    std::uniform_int_distribution<int> dis(0, roulette_arr.size());
    for (int i = 0; i < size; i++){
        indexes.push_back(dis(mt_generator));
    }
    return indexes;
}

std::vector<chromosome_t> crossover(std::vector<chromosome_t> parents) {
    chromosome_t child1;
    chromosome_t child2;
    std::vector<chromosome_t> offspring;
    std::uniform_int_distribution<int> dis(0, 99);
    int cross_p = dis(mt_generator);

    for (int i = 0; i < cross_p; i++){
        child1.push_back(parents[0][i]);
        child2.push_back(parents[1][i]);
    }

    for (int i = cross_p; i < 100; i++){
        child1.push_back(parents[1][i]);
        child2.push_back(parents[0][i]);
    }
    offspring.push_back(child1);
    offspring.push_back(child2);
    return offspring;
}

chromosome_t mutation(chromosome_t chromosome, double p_mutation) {
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    if (uniform(mt_generator) < p_mutation){
        std::uniform_int_distribution<int> mut(0, 99);
        if (chromosome[mut(mt_generator)] == 1){
            chromosome[mut(mt_generator)] = 0;
        }else chromosome[mut(mt_generator)] = 1;
    }
    return chromosome;
}

auto ackley_f = [](std::pair<double, double> pair) {
    return -20.0 * exp(-0.2 * sqrt(0.5 * (pow(pair.first, 2) + pow(pair.second, 2)))) -
           exp(0.5 * (cos(2 * M_PI * pair.first) + cos(2 * M_PI * pair.second))) + exp(1) + 20;
};

double fitness_function(chromosome_t &chromosome) {
    return 1.0 / (1.0 + abs(ackley_f(decode(chromosome))));
}

std::vector<double> fitness_population(population_t &population) {
    using namespace std;
    vector<double> population_fit;
    for (int i = 0; i < population.size(); i++){
        population_fit.push_back(fitness_function(population[i]));
    }
    return population_fit;
}

bool term_condition(population_t &population, std::vector<double> population_fit){
    bool found_good_fit = false;
    for (int i = 0; i < population_fit.size(); i++){
        if (population_fit[i] == 1){
            found_good_fit = true;
        }
    }
    return found_good_fit;
}

int main() {
    using namespace std;
    population_t population = population_generator(100 + (23366 % 10) * 2);
    auto result = genetic_algorithm(population,
                                    fitness_population,
                                    term_condition,
                                    selection, 1.0,
                                    crossover, 0.01, mutation);

    for (chromosome_t &chromosome: result) {
        pair<double, double> decoded = decode(chromosome);
        if (fitness_function(chromosome) == 1) {
            cout << "----------------------------------------------------------------------------------;" << endl;
            cout << "X: " << decoded.first << " Y: " << decoded.second << " | FITNESS: " <<
                fitness_function(chromosome)<< endl;
            cout << "----------------------------------------------------------------------------------;" << endl;
        }else{
            cout << "x: " << decoded.first << " y: " << decoded.second << " | fitness: " << fitness_function(chromosome)
                 << endl;
        }
    }
    cout << endl;
    return 0;
}