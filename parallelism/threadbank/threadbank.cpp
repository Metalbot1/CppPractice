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

    threadbank testbank(4);
    std::cout << "Testbank initialized" << std::endl;

    job testjob;
    testjob.add_task(wait, std::make_tuple(10.0));
    testjob.add_task(wait, std::make_tuple(0.2));
    testjob.add_task(wait, std::make_tuple(0.6));
    testjob.add_task(wait, std::make_tuple(1.4));
    testjob.add_task(wait, std::make_tuple(6.0));
    testjob.add_task(wait, std::make_tuple(0.2));
    testjob.add_task(wait, std::make_tuple(0.6));
    testjob.add_task(wait, std::make_tuple(1.4));
    testjob.add_task(wait, std::make_tuple(6.0));
    std::cout << "Testjob built" << std::endl;

    testbank.add_job(std::make_shared<job>(testjob));
    std::cout << "Testjob added to queue" << std::endl;

    testbank.soft_rejoin();
    std::cout << "Testbank rejoined." << std::endl;

    return 0;
}

threadbank::threadbank(int num_workers){
    ///Initialize desired # of workers
    for(int wi = 0; wi < num_workers; wi++)
        worker_pool.emplace_back();

    manager = std::make_shared<std::thread>(std::thread(&threadbank::threadbank_main, this));
}

void threadbank::rejoin(){
    //Tell the manager to stop running
    bool run = false;

    //Wait until the manager rejoins all the workers
    while(!worker_pool.empty());

    //lock all mutexes.
    queue_mutex.lock();

    //Rejoin manager
    manager->join();
}

void threadbank::soft_rejoin(){
    std::cout << "Attempting soft rejoin." << std::endl;
    std::unique_lock<std::mutex> m_lk(master_mutex);

    //Wait until all jobs are finished
    if(!job_queue.empty()) {
        std::cout << "Job queue is not empty" << std::endl;

        //wait until the job queue is empty
        master_cv.wait(m_lk);
        m_lk.unlock();
    }

    std::cout << "Setting run to false and notifying manager" << std::endl;

    //Manager has notified master thread -> job queue is empty.
    run = false;
    queue_cv.notify_one();

    //Wait until the manager rejoins all the workers
    master_cv.wait(m_lk);

    std::cout << "Manager says that all workers are rejoined. Joining manager" << std::endl;

    //Rejoin manager
    manager->join();
}

void threadbank::threadbank_main(){
    std::cout << "Manager started." << std::endl;
    std::unique_lock<std::mutex> q_lk(queue_mutex);

    //wait until we've been given a job.
    queue_cv.wait(q_lk);
    q_lk.unlock();

    std::cout << "Manager noticed that the job queue has been updated." << std::endl;

    //set the workers to the given job.
    for(auto& w : worker_pool)
        w.set_current_job(job_queue.front());

    //Start all the workers
    for(auto& w : worker_pool)
        w.start_working();

    ///Manager loop
    while(run){
        q_lk.lock();
        if(job_queue.front()->is_done()) {
            job_queue.pop_front();
        }
        q_lk.unlock();

        //If the queue is empty, wait until we have another job.
        if(job_queue.empty()){
            std::cout << "Job queue is empty. Notifying master and then waiting for resume signal." << std::endl;
            master_cv.notify_one();
            queue_cv.wait(q_lk);
            std::cout << "Manager notified to continue" << std::endl;
        }else {
            //If it's not, make sure all the workers are working on the right job.
            for (auto &w : worker_pool) {
                //If this worker isnt working on it's intended job, set it to the correct job.
                if (w.get_current_job() != job_queue.front()) {
                    std::cout << "Fixing worker job." << std::endl;
                    w.set_current_job(job_queue.front());
                }
            }
        }
    }

    std::cout << "Manager out of main loop, attempting to rejoin all workers." << std::endl;

    ///Finished manager loop, so we want to rejoin all the workers.
    while(!worker_pool.empty()){
        worker_pool.back().rejoin();
        worker_pool.pop_back();
    }

    master_cv.notify_one();
}

void threadbank::add_job(std::shared_ptr<job> J){
    queue_mutex.lock();
    job_queue.push_back(J);
    queue_mutex.unlock();

    queue_cv.notify_one();
};


