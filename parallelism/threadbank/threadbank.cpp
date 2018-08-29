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

    worker test_worker_1, test_worker_2, test_worker_3, test_worker_4;

    test_worker_1.set_current_job(test_job_ptr);
    test_worker_2.set_current_job(test_job_ptr);
    test_worker_3.set_current_job(test_job_ptr);
    test_worker_4.set_current_job(test_job_ptr);

    test_worker_1.start_working();
    test_worker_2.start_working();
    test_worker_3.start_working();
    test_worker_4.start_working();

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

threadbank::threadbank(int num_workers){
    ///Create empty job so we can initlialize the worker pool
    job empty_job;
    job_queue.emplace_back(std::make_shared<job>(empty_job));

    ///Initialize desired # of workers
    for(int wi = 0; wi < num_workers; wi++)
        worker_pool.emplace_back(job_queue[0]);

    std::thread(threadbank_main);
}

void threadbank::threadbank_main(){

}




