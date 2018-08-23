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
};