#pragma once

#include "../../common.h"
#include "job.hpp"
#include "worker.h"

class threadbank {
public:
    explicit threadbank(int num_workers);

private:
    void threadbank_main();

    std::vector<worker> worker_pool;
    std::vector<std::shared_ptr<job> > job_queue;


};//*/

