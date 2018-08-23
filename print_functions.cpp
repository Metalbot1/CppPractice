#include "print_functions.h"

namespace print_functions {

    void print_linebreak(int size) {
        std::cout << std::string(size, '-') << std::endl;
    }

    void print_vector(const std::vector<int>& vec, std::string vecname) {
        std::cout << vecname << ": ";
        for (auto x : vec) {
            std::cout << std::to_string(x) << " ";
        }
        std::cout << std::endl;
    }

    void print_vector(const std::vector<double>& vec, std::string vecname) {
        std::cout << vecname << ": ";
        for (int vi = 0; vi < vec.size(); vi++) {
            std::cout << std::to_string(vec[vi]) << " ";
        }
        std::cout << std::endl;
    }

}
