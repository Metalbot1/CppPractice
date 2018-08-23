#pragma once

#include <iostream>
#include <vector>

namespace print_functions {
    void print_linebreak(int size);

    void print_vector(const std::vector<int>& vec, std::string vecname);
    void print_vector(const std::vector<double>& vec, std::string vecname);

} //namespace
