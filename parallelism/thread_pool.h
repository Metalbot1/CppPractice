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

    int task_create();
    bool task_finished(int task) const;
    void task_perform(int task, const Job& job);
    void task_wait(int task);

private:
    void worker_main();

    bool _running;
    mutable std::mutex _mutex;
    std::vector<std::thread> _threads;

    size_t _task_size;
    std::vector<int> _free_tasks; // "free" list for tasks. For reusing memory
    std::vector<unsigned int> _task_pending_count; // per-task count of how many jobs are pending

    size_t _job_size;
    std::vector<int> _job_task; // maps every job to the task it belongs to
    std::vector<Job> _job_function; // stores the functions linked to each job

    std::condition_variable _cv_worker;
    std::condition_variable _cv_master;
};