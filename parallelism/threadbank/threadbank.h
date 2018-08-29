#pragma once

#include "../../common.h"
#include "job.hpp"
#include "worker.h"

class threadbank {
public:
    explicit threadbank(int num_workers);

    void add_job(std::shared_ptr<job> J);

    void rejoin();

    void soft_rejoin();

private:
    void threadbank_main();

    std::vector<worker> worker_pool;
    std::list<std::shared_ptr<job> > job_queue;
    std::shared_ptr<std::thread> manager = nullptr;

    bool run;
    std::mutex queue_mutex, master_mutex;
    std::condition_variable queue_cv, master_cv;

};//*/

