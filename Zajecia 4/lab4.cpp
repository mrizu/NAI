#include <iostream>
#include <vector>
#include <random>

using chromosome_t = std::vector<int>;
using genotype_t = std::vector<chromosome_t>;
std::random_device rd;
std::mt19937 mt_generator(rd());

std::pair<double, double> decode(chromosome_t chromosome) {
    double x = 0.0;
    double y = 0.0;
    double base_x = 1.0;
    double base_y = 1.0;
    int half_c = chromosome.size() / 2;

    for (int i = 0; i < half_c; i++) {
        if (chromosome[i] == 1){
            x += base_x;
        }
        base_x = base_x * 2;
    }

    for (int i = half_c; i < chromosome.size(); i++) {
        if (chromosome[i] == 1){
            y += base_y;
        }
        base_y = base_y * 2;
    }
    x = x / pow(2.0, (half_c - 4)) - 8;
    y = y / pow(2.0, (half_c - 4)) - 8;

    return {x, y};
}

genotype_t genotype_generator(int size) {
    genotype_t genotype;
    for (int i = 0; i < size; i++){
        chromosome_t chromosome;
        for (int j = 0; j < 100; j++){
            std::uniform_int_distribution<int> byte(0, 1);
            chromosome.push_back(byte(mt_generator));
        }
        genotype.push_back(chromosome);
    }
    return genotype;
}

auto ackley_f = [](std::pair<double, double> pair) {
    return -20.0 * exp(-0.2 * sqrt(0.5 * (pow(pair.first, 2) + pow(pair.second, 2)))) -
           exp(0.5 * (cos(2 * M_PI * pair.first) + cos(2 * M_PI * pair.second))) + exp(1) + 20;
};

double fitness_function(chromosome_t &chromosome) {
    return 1.0 / ackley_f(decode(chromosome));
}

int main() {
    using namespace std;
    genotype_t genotype = genotype_generator(100 + (23366 % 10) * 2);

    for (chromosome_t &chromosome: genotype) {
        pair<double, double> decoded = decode(chromosome);
       cout << "x: "<< decoded.first << " y: "<<  decoded.second << " | fitness: " << fitness_function(chromosome) << endl;
    }

    return 0;
}
