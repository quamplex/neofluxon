#ifndef NF_FOREGROUND_THREAD_POOL_H
#define NF_FOREGROUND_THREAD_POOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <vector>
#include <atomic>
#include <memory>
#include <cstddef>

// Forward declaration
class NfTask;

namespace NfCore {

/**
 * @brief Thread pool optimized for foreground tasks with priority support.
 * 
 * Uses std::jthread (C++20) for automatic join-on-destruction and stop tokens.
 * Manages NfTask objects via unique_ptr.
 */
class NfForegroundThreadPool {
public:
    explicit NfForegroundThreadPool(size_t threadCount = 0);
    ~NfForegroundThreadPool();

    // Non-copyable, non-movable
    NfForegroundThreadPool(const NfForegroundThreadPool&) = delete;
    NfForegroundThreadPool& operator=(const NfForegroundThreadPool&) = delete;
    NfForegroundThreadPool(NfForegroundThreadPool&&) = delete;
    NfForegroundThreadPool& operator=(NfForegroundThreadPool&&) = delete;

    /**
     * @brief Submit a task to the pool.
     * 
     * @param task Unique pointer to the task. Ownership is transferred to the pool.
     * @param priority Higher numbers = higher priority (default 0).
     */
    void submit(std::unique_ptr<NfTask> task, int priority = 0);

    /**
     * @brief Wait for all currently queued and active tasks to complete.
     * 
     * Blocks the calling thread until m_pendingTasks and m_activeTasks reach zero.
     */
    void waitForAllTasks();

    /**
     * @brief Gracefully shut down the pool.
     * 
     * Stops accepting new tasks and waits for all workers to finish.
     */
    void shutdown();

    /**
     * @brief Get the number of pending tasks.
     */
    size_t getPendingCount() const { return m_pendingTasks.load(); }

    /**
     * @brief Get the number of currently executing tasks.
     */
    size_t getActiveCount() const { return m_activeTasks.load(); }

private:
    struct Job {
        std::unique_ptr<NfTask> task;
        int priority;

        // Priority queue orders by highest priority first
        bool operator<(const Job& other) const {
            return priority < other.priority;
        }
    };

    void workerLoop(std::stop_token stoken);

    // State
    std::atomic<bool> m_stopped{false};
    std::atomic<size_t> m_activeTasks{0};
    std::atomic<size_t> m_pendingTasks{0};

    // Workers
    std::vector<std::jthread> m_workers;

    // Synchronization
    mutable std::mutex m_queueMutex;
    std::priority_queue<Job> m_tasks;
    std::condition_variable_any m_condVar;
    std::condition_variable_any m_completionCond;
};

} // namespace NfCore

#endif // NF_FOREGROUND_THREAD_POOL_H
