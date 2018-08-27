#pragma once

#include <queue>
#include "../../common.h"

///task_type is a non-template class for creating the task vector in job
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

///Contains and controls a vector of tasks
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

    template<class... Args>
    void add_task(void (*f)(Args...), std::tuple<Args...> a){
        add_task(task<Args...>(f, a));
    }

    void run_all_tasks(){
        while(!is_done())
            run_next_task();
    }

    void run_next_task() {
        if (!is_done()) {
            std::shared_ptr<task_type> task_holder = tasks.front();
            tasks.pop_front();
            task_holder->run();
        }else{
            std::cout << "Cannot run next task, no tasks left in job queue" << std::endl;
        }
    }

    bool is_done(){
        return tasks.empty();
    }
private:
    std::list<std::shared_ptr<task_type> > tasks;
};
