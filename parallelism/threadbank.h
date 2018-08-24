#pragma once

#include "../common.h"

///Jobs contain tasks
template<class Function, class... Args>
class task{
public:
    explicit task(Function (*f)(Args...), std::tuple<Args...> a): func(f), args(a){}

    Function run() {
        return func(std::get<Args...>(args));
    }

    Function (*func)(Args...);
    std::tuple<Args...> args;
};

class job{
    ///Job class will hold the queue of tasks needed to be completed
};

class threadbank {
public:

private:

};//*/

