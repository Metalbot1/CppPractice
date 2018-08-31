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
    job() = default;

    job(const job& J): tasks(J.tasks){};

    ~job() = default;

    template<class... Args>
    void add_task(std::shared_ptr<task<Args...> > t){
        tasks_mutex.lock();
        tasks.push_back(t);
        tasks_mutex.unlock();
    }

    template<class... Args>
    void add_task(const task<Args...>& t){
        add_task(std::make_shared<task<Args...> >(t));
    }

    template<class... Args>
    void add_task(void (*f)(Args...), std::tuple<Args...> a){
        add_task(task<Args...>(f, a));
    }

    template<class... Args>
    void add_task(void (*f)(Args...), Args... a){
        add_task(f, std::tuple<Args...>(a...));
    }

    void run_all_tasks(){
        while(!is_done())
            run_next_task();
    }

    void run_next_task() {
        if (!is_done()) {
            //Lock the tasks mutex, to ensure that noone else modifies the tasks list at the same time
            tasks_mutex.lock();

            //Store next task in queue and pop it.
            std::shared_ptr<task_type> task_holder = tasks.front();
            tasks.pop_front();

            //Unlock the task mutex and then run the task.
            tasks_mutex.unlock();
            task_holder->run();
        }else{
            std::cout << "Cannot run next task, no tasks left in job queue" << std::endl;
        }
    }

    bool is_done(){
        //std::cout << tasks.empty();
        tasks_mutex.lock();
        bool done = tasks.empty();
        tasks_mutex.unlock();
        //std::cout << done << std::endl;
        return done;
    }

    std::mutex tasks_mutex;
private:
    std::list<std::shared_ptr<task_type> > tasks;
};
