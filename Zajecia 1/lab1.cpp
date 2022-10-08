#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using chosen_function = std::function<double(std::vector<double>)>;

void outputResult(std::vector<double> numbers, chosen_function func){
    using namespace std;
    cout << func(numbers);
}

int main(int argc, char **argv) {
    using namespace std;
    map<string, chosen_function> math_func;
    math_func["add"] = [](vector<double> x) {return x.at(0) + x.at(1);};
    math_func["sin"] = [](vector<double> x) {return sin(x.at(0));};
    math_func["mod"] = [](vector<double> x) {return (int)x.at(0) % (int)x.at(1);};

    try {
        vector<string> arguments(argv, argv + argc);
        vector<double> func_arguments;
        for(int i = 2;i < arguments.size();i++){
            func_arguments.push_back(stod(arguments.at(i)));
        }
        auto selected_f = arguments.at(1);
        outputResult(func_arguments, math_func.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "Please enter parameters in format: [FUNCTION NAME] [ARGUMENT] ..." << endl;
        cout << "Available functions: add, sin, mod" << endl;
    }

    return 0;
}