#include <iostream>
#include <vector>
#include <random>

using genotype_t = std::vector<int>;
using chromosome_t = std::vector<genotype_t>;
std::random_device rd;
std::mt19937 mt_generator(rd());

std::pair<double, double> decode(genotype_t genotype) {
    double x = 0.0;
    double y = 0.0;
    double base_x = 1.0;
    double base_y = 1.0;
    int half_c = genotype.size() / 2;

    for (int i = 0; i < half_c; i++) {
        if (genotype[i] == 1){
            x += base_x;
        }
        base_x = base_x * 2;
    }

    for (int i = half_c; i < genotype.size(); i++) {
        if (genotype[i] == 1){
            y += base_y;
        }
        base_y = base_y * 2;
    }
    x = x / pow(2.0, (half_c - 4)) - 8;
    y = y / pow(2.0, (half_c - 4)) - 8;

    return {x, y};
}

chromosome_t chromosome_generator(int size) {
    chromosome_t chromosome;
    for (int i = 0; i < size; i++){
        genotype_t genotype;
        for (int j = 0; j < 100; j++){
            std::uniform_int_distribution<int> byte(0, 1);
            genotype.push_back(byte(mt_generator));
        }
        chromosome.push_back(genotype);
    }
    return chromosome;
}

auto ackley_f = [](std::pair<double, double> pair) {
    return -20.0 * exp(-0.2 * sqrt(0.5 * (pow(pair.first, 2) + pow(pair.second, 2)))) -
           exp(0.5 * (cos(2 * M_PI * pair.first) + cos(2 * M_PI * pair.second))) + exp(1) + 20;
};

double fitness_function(genotype_t &genotype) {
    return 1.0 / ackley_f(decode(genotype));
}

int main() {
    using namespace std;
    chromosome_t chromosome = chromosome_generator(100 + (23366 % 10) * 2);

    for (genotype_t &genotype: chromosome) {
        pair<double, double> decoded = decode(genotype);
       cout << "x: "<< decoded.first << " y: "<<  decoded.second << " | fitness: " << fitness_function(genotype) << endl;
    }

    return 0;
}
