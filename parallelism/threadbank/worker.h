#pragma once

#include <queue>
#include "../../common.h"
#include "job.hpp"

class worker{
public:
    //TODO: Constructor to initialize a local thread and run worker_main on it.
    worker();

    //TODO: Function to tell the worker to stop
    void stop_running();

    //TODO: Tell the worker to work on a specific job
    void add_job(job J);

    //TODO: Getter for the working boolean
    bool is_working(){return working;};

private:
    bool working, run;

    //TODO: Implement main function for the worker.
    void worker_main();
};