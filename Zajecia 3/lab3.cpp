#include <iostream>
#include <random>
#include <cmath>
#include <functional>
#include <chrono>

std::random_device rd;
std::mt19937 mt_generator(rd());

auto coords_generator = [](double minR, double maxR) {
    std::uniform_real_distribution<> coords(minR, maxR);
    return std::pair<double, double>(coords(mt_generator), coords(mt_generator));
};

auto brute_force = [](auto f, auto domain, int iterations, double minR, double maxR) {
    auto current_p = domain(minR, maxR);
    auto best_point = current_p;

    for (int i = 0; i < iterations; ++i) {
        if (f(current_p) < f(best_point)) {
            best_point = current_p;
        }
        current_p = domain(minR, maxR);
    }

    return best_point;
};

auto hill_climbing = [](auto f, auto domain, int max_iterations, double minimal_d, double maximal_d) {
    std::pair<double, double> current_p = domain(minimal_d, maximal_d);

    for (int iteration = 0; iteration < max_iterations; iteration++) {
        std::pair<double, double> new_p;
        std::pair<double, double> neighbour_p = domain(minimal_d/100, maximal_d/100);
        new_p.first = current_p.first + neighbour_p.first;
        new_p.second = current_p.second + neighbour_p.second;

        if (f(current_p) > f(new_p)) {
            current_p = new_p;
        }
    }
    return current_p;
};

auto simulated_annealing = [](auto f, auto domain, int max_iterations, double minimal_d, double maximal_d) {

    std::vector<std::pair<double, double>> pairsVector;
    std::uniform_real_distribution<double> uk_dis(0, 1);
    double uk = uk_dis(mt_generator);

    auto best_point = domain(minimal_d, maximal_d);
    pairsVector.push_back(best_point);

    for (int i = 0; i < max_iterations; ++i) {
        auto tk = domain(minimal_d, maximal_d);
        if (f(tk) <= f(best_point)) {
            best_point = tk;
            pairsVector.push_back(best_point);
        } else if (uk < exp(-(std::abs(f(tk) - f(best_point)) / (1 / log(i))))) {
            best_point = tk;
            pairsVector.push_back(best_point);
        }
    }
    return best_point;
};

void print(int iterations, auto ackley_f, auto himmelblau_f, auto booth_f){
    std::vector<std::function<double(std::pair<double, double> pair)>> func = {ackley_f, himmelblau_f, booth_f};
    std::vector<std::string> f_name = {"Ackley", "Himmelblau", "Booth"};
    std::vector<std::pair<double, double>> domain_range = {{-5, 5}, {-10, 10}, {-5, 5}};

    std::cout << "=======================================================================================" << std::endl;
    std::cout << "Brute force" << std::endl;
    std::cout << "=======================================================================================" << std::endl;
    for (int i = 0; i < 3; i++){
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        auto best_point = brute_force(func[i], coords_generator, iterations,
                                      domain_range[i].first, domain_range[i].second);
        auto stop = std::chrono::high_resolution_clock::now();
        std::cout << "Best " << f_name[i] << " x = " << best_point.first << " y = " << best_point.second
                  << " | result: " << func[i](best_point)
                  << " | time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                  << std::endl;
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    }

    std::cout << "=======================================================================================" << std::endl;
    std::cout << "Hill Climbing" << std::endl;
    std::cout << "=======================================================================================" << std::endl;

    for (int i = 0; i < 3; i++){
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        auto best_point = hill_climbing(func[i], coords_generator, iterations,
                                      domain_range[i].first, domain_range[i].second);
        auto stop = std::chrono::high_resolution_clock::now();
        std::cout << "Best " << f_name[i] << " x = " << best_point.first << " y = " << best_point.second
                  << " | result: " << func[i](best_point)
                  << " | time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                  << std::endl;
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    }

    std::cout << "=======================================================================================" << std::endl;
    std::cout << "Simulated Annealing" << std::endl;
    std::cout << "=======================================================================================" << std::endl;

    for (int i = 0; i < 3; i++){
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        auto best_point = simulated_annealing(func[i], coords_generator, iterations,
                                        domain_range[i].first, domain_range[i].second);
        auto stop = std::chrono::high_resolution_clock::now();
        std::cout << "Best " << f_name[i] << " x = " << best_point.first << " y = " << best_point.second
                  << " | result: " << func[i](best_point)
                  << " | time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                  << std::endl;
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    }
}

int main() {
    auto ackley_f = [](std::pair<double, double> pair) {
        return -20.0 * exp(-0.2 * sqrt(0.5 * (pow(pair.first, 2) + pow(pair.second, 2)))) -
               exp(0.5 * (cos(2 * M_PI * pair.first) + cos(2 * M_PI * pair.second))) + exp(1) + 20;
    };

    auto himmelblau_f = [](std::pair<double, double> pair) {
        return pow((pow(pair.first, 2) + pair.second - 11), 2) +
               pow((pair.first + pow(pair.second, 2) - 7), 2);
    };

    auto booth_f = [](std::pair<double, double> pair) {
        return pow((pair.first + 2 * pair.second - 7), 2) +
               pow((2 * pair.first + pair.second - 5), 2);
    };

    print(100000,ackley_f, himmelblau_f, booth_f);
    return 0;
}
