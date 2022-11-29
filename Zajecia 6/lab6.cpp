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
        auto crossover, double p_mutation, auto mutation, int iterations, bool showInfo, auto printInfo) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0, 1.0);
    population_t population = initial_population;
    vector<double> population_fit = fitness(population);

    while (iterations > 0) {
        vector<int> parents_indexes(population.size());
        population_t new_population(population.size());
        transform(population_fit.begin(), population_fit.end(),
                  parents_indexes.begin(),
                  [&](auto e) { return selection(population_fit); });

        for (int i = 0; i < parents_indexes.size(); i += 2) {
            population_t offspring = {population[i], population[i + 1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            new_population[i] = offspring[0];
            new_population[i + 1] = offspring[1];
        }
        for (auto &chromosome: new_population) {
            chromosome = mutation(chromosome, p_mutation);
        }
        population = new_population;
        population_fit = fitness(population);
        if (showInfo){
            printInfo(population);
        }
        iterations--;
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
        for (int j = 0; j < 100 + (23366 % 10) * 2; j++) {
            std::uniform_int_distribution<int> byte(0, 1);
            chromosome.push_back(byte(mt_generator));
        }
        population.push_back(chromosome);
    }
    return population;
}

std::vector<chromosome_t> crossover(std::vector<chromosome_t> parents) {
    using namespace std;
    uniform_int_distribution<int> locus(0,parents.at(0).size()-1);
    int a = locus(mt_generator);
    int b = locus(mt_generator);
    if (a > b) swap(a,b);
    auto children = parents;
    for (int i = a; i < b; i++) {
        swap(children[0][i],children[1][i]);
    }
    return children;
}

chromosome_t mutation(chromosome_t chromosome, double p_mutation) {
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    for (int i = 0; i < chromosome.size();i++){
        if (uniform(mt_generator) < p_mutation){
            if (chromosome[i] == 1){
                chromosome[i] = 0;
            }else chromosome[i] = 1;
        }
    }

    return chromosome;
}

int selection(std::vector<double> fitnesses) {
    std::uniform_int_distribution<int> uniform(0, fitnesses.size()-1);
    int a = uniform(mt_generator);
    int b = uniform(mt_generator);
    return (fitnesses[a]>fitnesses[b])?a:b;
}

auto ackley_f = [](std::pair<double, double> pair) {
    return -20.0 * exp(-0.2 * sqrt(0.5 * (pow(pair.first, 2) + pow(pair.second, 2)))) -
           exp(0.5 * (cos(2 * M_PI * pair.first) + cos(2 * M_PI * pair.second))) + exp(1) + 20;
};

double fitness_function(chromosome_t &chromosome) {
    return 1.0 / (1.0 + ackley_f(decode(chromosome)));
}

std::vector<double> fitness_population(population_t &population) {
    using namespace std;
    vector<double> population_fit;
    for (int i = 0; i < population.size(); i++){
        population_fit.push_back(fitness_function(population[i]));
    }
    return population_fit;
}

double getPopulationAvarage(population_t &population){
    std::vector<double> population_fit = fitness_population(population);
    if(population_fit.empty()){
        return 0;
    }
    return std::reduce(population_fit.begin(), population_fit.end()) / population_fit.size();
}

auto getPopulationMax(population_t population){
    std::vector<double> population_fit = fitness_population(population);
    return *std::max_element(population_fit.begin(), population_fit.end());
}

auto getPopulationMin(population_t population){
    std::vector<double> population_fit = fitness_population(population);
    return *std::min_element(population_fit.begin(), population_fit.end());
}

void printPopulationInfo(population_t population){
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
    std::cout << "Population Avg: " << getPopulationAvarage(population) << std::endl;
    std::cout << "Population Min: " << getPopulationMin(population) << std::endl;
    std::cout << "Population Max: " << getPopulationMax(population) << std::endl;
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
}

bool term_condition(population_t &population, std::vector<double> population_fit){
    bool found_good_fit = false;
    for (int i = 0; i < population_fit.size(); i++){
        if (population_fit[i] >= 0.9){
            found_good_fit = true;
        }
    }
    return found_good_fit;
}

void printResult(population_t &population){
    using namespace std;
    for (chromosome_t &chromosome: population) {
        pair<double, double> decoded = decode(chromosome);
        if (fitness_function(chromosome) >= 0.9) {
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "X: " << decoded.first << " Y: " << decoded.second << " | FITNESS: " <<
                 fitness_function(chromosome)<< endl;
            cout << "----------------------------------------------------------------------------------" << endl;
        }else{
            cout << "x: " << decoded.first << " y: " << decoded.second << " | fitness: " << fitness_function(chromosome)
                 << endl;
        }
    }
    cout << "----------------------------------------------------------------------------------" << endl;
}

int main(int argc, char *argv[]) {
    using namespace std;
    int pop_size = stoi(argv[1]);
    int iterations = stoi(argv[2]);
    double p_crossover = stod(argv[3])/100;
    double p_mutation = stod(argv[4])/100;
    bool showInfo = stoi(argv[5]);

    population_t population = population_generator(pop_size);
    auto result = genetic_algorithm(population,
                                    fitness_population,
                                    term_condition,
                                    selection, p_crossover,
                                    crossover, p_mutation, mutation,
                                    iterations, showInfo, printPopulationInfo);

    printResult(result);
    return 0;
}