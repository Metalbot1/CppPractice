#pragma once

#include <queue>
#include "../common.h"

class task_type{
public:
    //Empty class for pointer purposes
    virtual void run();
};

///Jobs contain tasks
template<class... Args>
class task: public task_type{
public:
    explicit task(void (*f)(Args...), std::tuple<Args...> a): func(f), args(a){}

    void run() override{
        return run_from_tuple(args, std::index_sequence_for<Args...>());
    }

private:
    ///From https://www.murrayc.com/permalink/2015/12/05/modern-c-variadic-template-parameters-and-tuples/
    template<std::size_t... Is>
    void run_from_tuple(const std::tuple<Args...>& tuple, std::index_sequence<Is...>) {
        func(std::get<Is>(tuple)...);
    }

    void (*func)(Args...);
    std::tuple<Args...> args;
};

class job{
public:
    template<class... Args>
    void add_task(std::shared_ptr<task<Args...> > t){
        tasks.push_back(t);
    }

    template<class... Args>
    void add_task(const task<Args...>& t){
        add_task(std::make_shared<task<Args...> >(t));
    }

    void run_tasks(){
        for(const std::shared_ptr<task_type>& t : tasks){
            t->run();
        }
    }
private:
    std::vector<std::shared_ptr<task_type> > tasks;
};

class threadbank {
public:

private:

};//*/

