/**
 * File name: NfForegroundThreadPool.cpp
 * Project: Neofluxon (a photography workflow software)
 *
 * Copyright (C) 2026 Iurie Nistor
 *
 * This file is part of Neofluxon.
 *
 * Neofluxon is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "NfForegroundThreadPool.h"
#include "NfLogger.h"

namespace NfCore {

NfForegroundThreadPool::ForegroundThreadPool(size_t threadCount)
        : m_stopped {false}
{
        if (threadCount < 1) {
                threadCount = std::thread::hardware_concurrency();
                if (threadCount < 1)
                        threadCount = 1;
        }

        for (size_t i = 0; i < threadCount; i++) {
                m_workers.emplace_back([this](std::stop_token stoken) {
                        workerLoop(stoken);
                });
        }
}

NfForegroundThreadPool::~ForegroundThreadPool()
{
        shutdown();
}

void NfForegroundThreadPool::shutdown()
{
        m_stopped.store(true, std::memory_order_release);

        for (auto& t : m_workers)
                t.request_stop();

        m_condVar.notify_all();
}

void NfForegroundThreadPool::workerLoop(std::stop_token stoken)
{
        while (true) {
                Job job;
                {
                        std::unique_lock lock(m_queueMutex);

                        m_condVar.wait(lock, [&] {
                                return stoken.stop_requested() || !m_tasks.empty();
                        });

                        if (stoken.stop_requested())
                                return;

                        job = std::move(m_tasks.top());
                        m_tasks.pop();
                }

                try {
                        job.task();
                } catch (const std::exception& e) {
                        NfLogger::Error() << "Job failed: " << e.what();
                } catch (...) {
                        NfLogger::Error() << "Job failed with unknown exception";
                }
        }
}


        void ThreadPool::workerFunction() {
    while (true) {
        std::unique_ptr<NfTask> task;

        // 1. Wait for a task
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_condition.wait(lock, [this] { return m_stop || !m_taskQueue.empty(); });
            
            if (m_stop && m_taskQueue.empty()) return;
            
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
            m_activeTasks++;
        }

        // 2. Execute the task logic (runs in the worker thread)
        if (task) {
            TaskStatus status = task->execute(); 

            // 3. 🚨 HERE: Notify completion
            // This triggers the callback you defined in NfPhotoLoader::requestThumbnail
            // The callback runs in THIS worker thread (not the main thread!)
            task->notifyCompletion(status); 

            // 4. Cleanup counters
            {
                std::lock_guard<std::mutex> lock(m_queueMutex);
                m_activeTasks--;
                m_pendingTasks--;
            }
            m_completionCondition.notify_all();
        }
    }
}void ThreadPool::workerFunction() {
    while (true) {
        std::unique_ptr<NfTask> task;

        // 1. Wait for a task
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_condition.wait(lock, [this] { return m_stop || !m_taskQueue.empty(); });
            
            if (m_stop && m_taskQueue.empty()) return;
            
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
            m_activeTasks++;
        }

        // 2. Execute the task logic (runs in the worker thread)
        if (task) {
            TaskStatus status = task->execute(); 

            // 3. 🚨 HERE: Notify completion
            // This triggers the callback you defined in NfPhotoLoader::requestThumbnail
            // The callback runs in THIS worker thread (not the main thread!)
            task->notifyCompletion(status); 

            // 4. Cleanup counters
            {
                std::lock_guard<std::mutex> lock(m_queueMutex);
                m_activeTasks--;
                m_pendingTasks--;
            }
            m_completionCondition.notify_all();
        }
    }
}

} // namespace NfCore
