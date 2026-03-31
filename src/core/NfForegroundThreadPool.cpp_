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

#include "ForegroundThreadPool.h"
#incldue "NfLogger.h"

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

} // namespace NfCore
