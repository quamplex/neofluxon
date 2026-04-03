/**
 * File name: NfTask.h
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

#ifndef NF_TASK_H
#define NF_TASK_H

#include <memory>
#include <functional>
#include <string>

namespace NfCore {

class NfTask {
public:

     enum class TaskStatus {
                     TaskSuccess,
                     TaskFailed,
                     TaskCancelled
     };

     using TaskResultHandler = std::function<void(std::unique_ptr<NfTask>, TaskStatus)>;
     virtual ~NfTask() = default;
     virtual TaskStatus execute() = 0;
     void setResult(TaskResultHandler handler);
     void notifyCompletion(TaskStatus status = TaskStatus::Success);
     [[nodiscard]] bool hasResultHandler() const;

protected:
    NfTask() = default;

private:
    TaskResultHandler m_onComplete;
};

} // namespace NfCore

#endif // NF_TASK_H
