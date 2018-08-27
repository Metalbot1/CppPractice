#include "threadbank.h"

void add_one(int& num){
    num++;
}

void multiply(int& num, int factor){
    num = num*factor;
}

void print(int& num){
    std::cout << std::to_string(num) << std::endl;
}

int main(){

    job test_job;

    std::cout << "Job initialized" << std::endl;

    int counter = 476;

    test_job.add_task(add_one, std::make_tuple(std::ref(counter)));
    test_job.add_task(print, std::make_tuple(std::ref(counter)));
    test_job.add_task(add_one, std::make_tuple(std::ref(counter)));
    test_job.add_task(print, std::make_tuple(std::ref(counter)));
    test_job.add_task(add_one, std::make_tuple(std::ref(counter)));
    test_job.add_task(print, std::make_tuple(std::ref(counter)));
    test_job.add_task(add_one, std::make_tuple(std::ref(counter)));
    test_job.add_task(print, std::make_tuple(std::ref(counter)));

    std::cout << "Job filled" << std::endl;

    worker test_worker(std::make_shared<job>(test_job));

    test_worker.rejoin();

    return 0;
}




