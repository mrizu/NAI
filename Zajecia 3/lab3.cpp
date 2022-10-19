#include <iostream>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 mt_generator(rd());

//auto brute_force = [](auto f, auto domain, int iterations, double r1, double r2) {
//    auto current_p = domain(r1, r2);
//    auto best_point = current_p;
//
//    for (int i = 0; i < iterations; ++i) {
//        if (f(current_p) < f(best_point)) {
//            best_point = current_p;
//        }
//        current_p = domain(r1, r2);
//    }
//
//    return best_point;
//};

auto coords_generator(double r1, double r2) {
    std::uniform_real_distribution<> coords(r1, r2);
    return std::pair<double, double>(coords(mt_generator), coords(mt_generator));
}

int main() {
    auto ackley_f = [](std::pair<double, double> pair) {
        return 1; //TODO
    };
    auto himmelblau_f = [](std::pair<double, double> pair) {
        return pow((pow(pair.first,2) + pair.second - 11), 2) +
            pow((pair.first + pow(pair.second, 2) - 7), 2);
    };

    auto booth_f = [](std::pair<double, double> pair) {
        return pow((pair.first + 2 * pair.second - 7), 2) +
               pow((2 * pair.first + pair.second - 5), 2);
    };

    return 0;
}
