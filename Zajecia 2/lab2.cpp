#include <iostream>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 mt_generator(rd());

auto brute_force = [](auto f, auto domain, int iterations, double r1, double r2) {
    auto current_p = domain(r1, r2);
    auto best_point = current_p;

    for (int i = 0; i < iterations; ++i) {
        if (f(current_p) < f(best_point)) {
            best_point = current_p;
        }
        current_p = domain(r1, r2);
    }

    return best_point;
};

auto coords_generator(double r1, double r2) {
    std::uniform_real_distribution<> coords(r1, r2);
    return std::pair<double, double>(coords(mt_generator), coords(mt_generator));
}

int main() {
    auto beale_f = [](std::pair<double, double> pair) {
        return pow(1.5 - pair.first + pair.first * pair.second, 2) +
               pow(2.25 - pair.first + pair.first * pow(pair.second, 2), 2) +
               pow(2.625 - pair.first + pair.first * pow(pair.second, 3), 2);
    };

    auto booth_f = [](std::pair<double, double> pair) {
        return pow((pair.first + 2 * pair.second - 7), 2) +
               pow((2 * pair.first + pair.second - 5), 2);
    };

    auto matyas_f = [](std::pair<double, double> pair) {
        return 0.26 * (pow(pair.first, 2) +
                       pow(pair.second, 2) - 0.48 * pair.first * pair.second);
    };
    std::cout << "-------------------------------------------------------------------" << std::endl;
    auto best_beale = brute_force(beale_f, coords_generator, 2000000, -4.5, 4.5);
    std::cout << "Best Beale x = " << best_beale.first << " y = " << best_beale.second
              << " | result: " << beale_f(best_beale)
              << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;
    auto best_booth = brute_force(booth_f, coords_generator, 2000000, -10, 10);
    std::cout << "Best Booth x = " << best_booth.first << " y = " << best_booth.second
              << " | result: " << booth_f(best_booth)
              << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;
    auto best_matyas = brute_force(matyas_f, coords_generator, 2000000, -10, 10);
    std::cout << "Best Matyas x = " << best_matyas.first << " y = " << best_matyas.second
              << " | result: " << matyas_f(best_matyas)
              << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;

    return 0;
}
