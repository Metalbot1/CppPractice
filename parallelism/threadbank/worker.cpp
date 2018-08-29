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
    stop();

    working.lock(); //Wait until the worker is done working

    worker_thread->join(); //Rejoin the worker thread
}

//Main worker function, runs loop and tries to finish tasks for the current job.
void worker::worker_main(){
    std::cout << "Started worker_main" << std::endl;

    while(run){
        if (!current_job->is_done()){
            working.lock();
            while(!current_job->is_done())
                current_job->run_next_task();
            working.unlock();
        }
    }
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