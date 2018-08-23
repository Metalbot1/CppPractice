///Practice implementation of a thread pool
/// This follows the tutorial at https://nax.io/thread-pool

#include "thread_pool.h"

ThreadPool::ThreadPool(): _running(true)
{
    // Determine the amount of threads to create.
    size_t thread_count;
    thread_count = std::thread::hardware_concurrency();
    if(thread_count == 0)
        thread_count = 1;

    // Prevents re-allocation during our emplace_back.
    _threads.reserve(thread_count);

    for(size_t i = 0; i < thread_count; ++i)
        _threads.emplace_back(&ThreadPool::worker_main, this);
}

ThreadPool::~ThreadPool()
{
    // For usage of more complicated lock functions
    std::unique_lock<std::mutex> lock(_mutex);

    //We're done
    _running = false;

    // Unlock our mutex and rejoin all workers.
    lock.unlock();
    for(auto& t : _threads)
        t.join();
}

void ThreadPool::worker_main()
{
    while(_running){
        //Here's where our worker code goes.
    }
}
