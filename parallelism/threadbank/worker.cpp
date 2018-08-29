#include "worker.h"

worker::worker(){
    worker_thread = nullptr;
    run = false;
};

worker::worker(std::shared_ptr<job> J){
    current_job = J;

    //Set worker_thread to nullptr.
    worker_thread = nullptr;
    run = false;
}

worker::worker(const worker& W){
    //Copy worker's job pointer
    current_job = W.current_job;

    //Copy worker's thread pointer
    worker_thread = W.worker_thread;
    run = W.run;
}

worker::~worker(){
    //If we havent rejoined yet.
    if(run) {
        rejoin();
    }
}

const void worker::rejoin(){
    //Tell the worker main loop to stop
    run = false;

    work_cv.notify_one();
    //work_mutex.lock(); //Lock the worker thread
    worker_thread->join(); //Rejoin the worker thread

    std::cout << "Worker rejoined" << std::endl;
}

//Main worker function, runs loop and tries to finish tasks for the current job.
void worker::worker_main(){
    std::cout << "Started worker_main" << std::endl;
    std::unique_lock<std::mutex> w_lk(work_mutex);
    w_lk.unlock();

    while(run){
        if(current_job->is_done()) {
            waiting = true;
            work_cv.wait(w_lk);
            work_mutex.unlock();
            waiting = false;
        }else{
            work_mutex.lock();
            current_job->run_next_task();
            work_mutex.unlock();
        }
    }

    std::cout << "Worker out of main loop" << std::endl;
}

const void worker::start_working(){
    if(!run) {
        run = true;

        //Initialize the worker thread and have it begin running worker_main
        worker_thread = std::make_shared<std::thread>(std::thread{&worker::worker_main, this});
    }else{
        std::cout << "Cannot start working, worker is already running" << std::endl;
    }
}