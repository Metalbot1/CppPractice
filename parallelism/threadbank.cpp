#include "threadbank.h"

void add_one(int& in){
    in++;
}

bool is_three(double in){
    return in==3.0;
}

int main(){
    int one = 15;

    auto add = task<void, int&>(add_one, std::make_tuple(std::ref(one)));

    add.run();

    auto is3 = task<bool, double>(is_three, std::make_tuple(2.5));

    std::cout << one << std::endl;

    std::cout << is3.run();

    return 0;
}

///Task Implementation (Simply holds a function and a set of arguments for execution later.)
/*template<class Function, class... Args>
task<Function, Args...>::task(Function (*f)(Args...), Args... a) {
    func = f;
    args = std::make_tuple(a...);
}//*/


