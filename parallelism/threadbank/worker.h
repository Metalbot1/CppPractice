#pragma once

#include "job.hpp"
#include "../../util/utilities.h"

class worker{
public:
    //Constructor initializes and thread and starts it on the worker_main loop
    worker();

    explicit worker(std::shared_ptr<job> J);

    //Destructor waits until worker is done working and then rejoins
    ~worker();

    worker(const worker&);

    //Checks wether the worker is currently working on a task or not.
    bool is_running(){return run;}

    //Gets the job
    std::shared_ptr<job> get_current_job(){return current_job;}

    void set_current_job(const std::shared_ptr<job>& new_job){
        work_mutex.lock();
        current_job = new_job;
        work_mutex.unlock();

        work_cv.notify_one();
    }

    const void rejoin();

    const void start_working();

    bool is_waiting(){
        return waiting;
    }

private:
    //working boolean is
    bool run, waiting;

    std::mutex work_mutex;
    std::condition_variable work_cv;

    //The thread that this worker runs on
    std::shared_ptr<std::thread> worker_thread = nullptr;

    //Current job
    std::shared_ptr<job> current_job;

    //Main worker loop, tries to complete current job
    void worker_main();
};

