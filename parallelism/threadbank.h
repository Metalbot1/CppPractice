#pragma once

#include "../common.h"

///Jobs contain tasks
template<class Function, class... Args>
class task{
public:
    explicit task(Function (*f)(Args...), std::tuple<Args...> a): func(f), args(a){}

    Function run() {
        return run_from_tuple(args, std::index_sequence_for<Args...>());
    }

private:
    ///From https://www.murrayc.com/permalink/2015/12/05/modern-c-variadic-template-parameters-and-tuples/
    template<std::size_t... Is>
    Function run_from_tuple(const std::tuple<Args...>& tuple, std::index_sequence<Is...>) {
        return func(std::get<Is>(tuple)...);
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

