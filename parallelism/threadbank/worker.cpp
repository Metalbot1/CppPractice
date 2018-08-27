#include "worker.h"

worker::worker(){
    //Set run to false because we want to allow external setup before starting the worker loop
    run = true;

    //Initialize the worker thread and have it begin running worker_main
    worker_thread = std::thread{&worker::worker_main, this};
}

worker::worker(std::shared_ptr<job> J){
    current_job = J;

    //Set run to false because we want to allow external setup before starting the worker loop
    run = true;

    //Initialize the worker thread and have it begin running worker_main
    worker_thread = std::thread{&worker::worker_main, this};

    //Give the worker some time to gain control of the mutex.
    clock_t starttime = std::clock();
    while ((std::clock() - starttime) / CLOCKS_PER_SEC < 0.001);
}

worker::~worker(){
    //If we havent rejoined yet.
    if(run) {
        rejoin();
    }
}

void worker::rejoin(){
    //Tell the worker main loop to stop
    stop();

    working.lock(); //Wait until the worker is done working

    worker_thread.join(); //Rejoin the worker thread

    std::cout << "Worker rejoined." << std::endl;
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