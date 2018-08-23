///Practice implementation of a thread pool.

#include "../common.h"

class ThreadPool
{
public:
    using Job = std::function<void(void)>;

    ThreadPool();
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    int task_create(); // reates a new task and adds it to the queue, returns it's ID.
    bool task_finished(int task) const; // Check if a task is finished
    void task_perform(int task, const Job& job); //
    void task_wait(int task);

private:
    void worker_main(); // Function run by all of the workers for the duration of their existence

    bool _running; // Bool to hold wether or not the ThreadPool is still running.
    mutable std::mutex _mutex; // Threadpool while mutex for synchronization
    std::vector<std::thread> _threads; // Vector of worker threads.

    size_t _task_size; // The amount of tasks
    std::vector<int> _free_tasks; /// "free" list for tasks. For reusing memory
    std::vector<unsigned int> _task_pending_count; // per-task count of how many jobs are pending

    size_t _job_size; // The amount of jobs currrently in the queue
    std::vector<int> _job_task; // The list of task_IDs that each job belongs to
    std::vector<Job> _job_function; // The functions that each job needs to execute.

    std::condition_variable _cv_worker; // For notifying workers to wake up
    std::condition_variable _cv_master; // For messages from workers to master thread.
};