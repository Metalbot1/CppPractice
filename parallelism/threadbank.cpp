#include "threadbank.h"

void add_one(int& in, int& out){
    out = in + 1;
}

bool is_three(double in){
    return in==3.0;
}

void are_equal(int a, int b, bool& eq){
    eq = a==b;
}

int main(){
    int num = 1;

    auto add = task<void, int&, int&>(add_one, std::make_tuple(std::ref(num), std::ref(num)));

    auto is3 = task<bool, double>(is_three, std::make_tuple(2.5));

    bool eq = false;

    auto areq = task<void, int, int, bool&>(are_equal, std::make_tuple(3, 3, std::ref(eq)));

    std::cout << num << std::endl;
    add.run();
    std::cout << num << std::endl;
    add.run();
    std::cout << num << std::endl;

    std::cout << is3.run() << std::endl;

    std::cout << eq << std::endl;
    areq.run();
    std::cout << eq;

    return 0;
}


