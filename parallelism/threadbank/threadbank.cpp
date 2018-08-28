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

void wait(double sec){
    clock_t starttime = std::clock();
    while ((std::clock() - starttime) / CLOCKS_PER_SEC < sec);
    std::cout << "Done waiting " << std::to_string(sec) << std::endl;
}

int main(){

    job test_job;

    std::cout << "Job initialized" << std::endl;

    int counter = 476;

    test_job.add_task(wait, std::make_tuple(10.0));
    test_job.add_task(wait, std::make_tuple(1.2));
    test_job.add_task(wait, std::make_tuple(0.5));
    test_job.add_task(wait, std::make_tuple(1.0));

    std::cout << "Job filled" << std::endl;

    ///Need to make external shared ptr to the job we want to execute, so the job isnt copied for each worker
    std::shared_ptr<job> test_job_ptr = std::make_shared<job>(test_job);

    worker test_worker_1(test_job_ptr);
    worker test_worker_2(test_job_ptr);
    worker test_worker_3(test_job_ptr);
    worker test_worker_4(test_job_ptr);

    while(!test_job_ptr->is_done());

    std::shared_ptr<job> test_job_ptr_2 = std::make_shared<job>(test_job);

    test_worker_1.set_current_job(test_job_ptr_2);
    test_worker_2.set_current_job(test_job_ptr_2);
    test_worker_3.set_current_job(test_job_ptr_2);
    test_worker_4.set_current_job(test_job_ptr_2);

    test_worker_2.rejoin();
    test_worker_1.rejoin();
    test_worker_3.rejoin();
    test_worker_4.rejoin();

    return 0;
}




