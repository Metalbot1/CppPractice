///Practice implementation of a thread pool
/// This follows the tutorial at https://nax.io/thread-pool

#include "thread_pool.h"

ThreadPool::ThreadPool(): _running(true), _task_size(0u), _job_size(0u)
{
    // Determine the amount of threads to create.
    size_t thread_count;
    thread_count = std::thread::hardware_concurrency();
    if(thread_count == 0)
        thread_count = 1;

    // Prevents re-allocation during our emplace_back.
    _threads.reserve(thread_count);

    //Run threads using worker_main
    for(size_t i = 0; i < thread_count; ++i)
        _threads.emplace_back(&ThreadPool::worker_main, this);
}

ThreadPool::~ThreadPool()
{
    // For usage of more complicated lock functions
    std::unique_lock<std::mutex> lock(_mutex);

    //We're done
    _running = false;

    //Wake up all workers so we can actually rejoin them
    _cv_worker.notify_all();

    // Unlock our mutex and rejoin all workers.
    lock.unlock();
    for(auto& t : _threads)
        t.join();
}

void ThreadPool::worker_main()
{
    std::unique_lock<std::mutex> lock(_mutex);

    while(_running){
        ///Wait until we are woken up by some external influence?
        _cv_worker.wait(lock);

        while(true){
            if(_job_size == 0)
                break; // If there's no jobs, exit this inner loop.

            int task = _job_task.back(); // The task is the one which the job at the end of our queue belongs to.
            Job job = _job_function.back(); // The job is the one at the end of queue

            // Remove the job that we're going to do from the queue
            _job_task.pop_back();
            _job_function.pop_back();
            _job_size--;

            // Unlock the mutex.
            lock.unlock();

            // Do the job
            job();

            // Lock the mutex
            lock.lock();
            _task_pending_count[task]--; // Let master know that we've reduced the job count for this task by one
            if(_task_pending_count[task] == 0)
                _cv_master.notify_all(); // Let master know that this task is completely finished.
        }
    }
}

//Not sure that I fully understand this function.
int ThreadPool::task_create(){

    std::lock_guard<std::mutex> lock(_mutex);
    int task;

    if(_free_tasks.empty()){ // if there are no "free" tasks?
        task = static_cast<int>(_task_size); // store our task_size in return variable (why?)
        _task_size++; // increase task_size (makes sense)
        _task_pending_count.resize(_task_size); // resizes the task_pending queue
    }else{
        task = _free_tasks.back(); // take the back element of free_tasks as our return variable (why?)
        _free_tasks.pop_back(); // remove final element of "free_tasks" (ie the one we are going to return.)
    }
    _task_pending_count[task] = 0u; // set the this task id to pending?

    return task; // really dont know what we're trying to return.
}

//Checks if the task identified by the input is completed. Simple.
bool ThreadPool::task_finished(int task) const{
    std::lock_guard<std::mutex> lock(_mutex);

    return _task_pending_count[task] == 0;
}

//Adds a job to a task.
void ThreadPool::task_perform(int task, const Job& job){
    std::lock_guard<std::mutex> lock(_mutex);

    _job_size++; // Not sure what job_size is for yet
    _job_task.push_back(task); //Record which task this job belongs to in our job_test vector.
    _job_function.push_back(job); //Record what function to run to complete this job.
    _task_pending_count[task]++; //Increment the record of how many jobs remain for this task
}

// For making the master wait until a task is completed.
void ThreadPool::task_wait(int task){
    std::unique_lock<std::mutex> lock(_mutex); //Lock variable for simpler mutex usage.

    while(_task_pending_count[task] > 0)
        _cv_master.wait(lock); //If we're not done, wait.

    _free_tasks.push_back(task); // Put the completed task ID into the _free_tasks variable... I dont know why.
}