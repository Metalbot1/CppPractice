#include "worker.h"

//Default constructor sets everything to null.
//Using this requires later setting a job and running worker::start_working
worker::worker(){
    worker_thread = nullptr;
    run = false;
};

//Construction with a job will set the job and start the worker immediately.
worker::worker(std::shared_ptr<job> J){
    current_job = J;

    start_working();
}

//Copy constructor just copies pointers (current_job and worker_thread)
worker::worker(const worker& W){
    //Copy worker's job pointer
    current_job = W.current_job;

    //Copy worker's thread pointer
    worker_thread = W.worker_thread;
}

//Destructor rejoins worker if it is still running.
worker::~worker(){
    //If we havent rejoined yet.
    if(run) {
        rejoin();
    }
}

//Rejoin the worker if it hasn't been rejoined already.
const void worker::rejoin(){
    if(run) {
        //Tell the worker main loop to stop
        run = false;

        work_cv.notify_one();
        worker_thread->join(); //Rejoin the worker thread

        std::cout << "Worker rejoined" << std::endl;
    }else{
        std::cout << "Cannot rejoin worker, already rejoined." << std::endl;
    }
}

//Main worker function, runs loop and tries to finish tasks for the current job.
void worker::worker_main(){
    std::cout << "Started worker_main" << std::endl;

    while(run){
        if(current_job->is_done()) {
            waiting = true;
            std::unique_lock<std::mutex> w_lk(work_mutex);
            work_cv.wait(w_lk);
            waiting = false;
        }else{
            work_mutex.lock();
            current_job->run_next_task();
            work_mutex.unlock();
        }
    }

    std::cout << "Worker out of main loop" << std::endl;
}

//Tells the worker thread to start working, if it isn't already working.
const void worker::start_working(){
    if(!run) {
        run = true;

        //Initialize the worker thread and have it begin running worker_main
        worker_thread = std::make_shared<std::thread>(std::thread{&worker::worker_main, this});
    }else{
        std::cout << "Cannot start working, worker is already running" << std::endl;
    }
}

//Set's the current job and notifies the worker in case it's waiting for intervention from the manager.
void worker::set_current_job(const std::shared_ptr<job>& new_job){
    work_mutex.lock();
    current_job = new_job;
    work_mutex.unlock();

    work_cv.notify_one();
}