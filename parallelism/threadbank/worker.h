#pragma once

#include "job.hpp"
#include "../../util/utilities.h"

class worker{
public:
    //Default constructor sets run flag to false and worker_thread NULL
    worker();

    //Construction with a job pointer makes the worker begin work immediately
    explicit worker(std::shared_ptr<job> J);

    //Destructor rejoins the worker thread if it hasnt been already.
    ~worker();

    //Copy constructor just copies pointers. DOES NOT CREATE A NEW WORKER THREAD!
    worker(const worker&);

    //Returns the run flag
    bool is_running(){return run;}

    //Returns the job pointer
    std::shared_ptr<job> get_current_job(){return current_job;}

    //Sets the job pointer, and notifies the worker thread to continue working
    void set_current_job(const std::shared_ptr<job>& new_job);

    //Rejoins the worker thread, if it hasnt been already
    const void rejoin();

    //Starts the worker, if it's not already running.
    const void start_working();

    //Returns wether or not the worker is currently waiting for intervention from a manager
    bool is_waiting(){return waiting;}

private:
    //Flags for the worker to communicate with the manager.
    bool run, waiting;

    //Synchronization variables for communication between worker and manager.
    std::mutex work_mutex;
    std::condition_variable work_cv;

    //Pointer to the worker thread.
    std::shared_ptr<std::thread> worker_thread = nullptr;

    //Pointer to the current job.
    std::shared_ptr<job> current_job;

    //Loop that the worker thread runs.
    void worker_main();
};

