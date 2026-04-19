/**
 * File name: NfPhotoLoader.cpp
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

#include "NfPhotoLoader.h"
#include "NfPathScanner.h"
#include "NfImage.h"
#include "NfThumbnailTask.h"
#include "NfPreviewTask.h"
#include "NfLogger.h"

namespace NfCore {

NfPhotoLoader::NfPhotoLoader()
        : m_pathScanner{std::make_unique<NfPathScanner>()}
        , m_generationId{0}
{
}

NfPhotoLoader::~NfPhotoLoader()
{
        NF_LOG_DEBUG("called");
}

void NfPhotoLoader::setPath(const std::filesystem::path &path)
{
        {
                std::scoped_lock lock(m_queueMutex);
                m_thumbnailsQueue.clear();
                m_generationId++;
        }

        m_path = path;
        m_pathScanner->setPath(path);
}

const std::filesystem::path& NfPhotoLoader::getPath() const
{
        return m_path;
}

void NfPhotoLoader::requestThumbnail(const NfPhoto &photo, std::unique_ptr<NfImage> image)
{
        auto task = std::make_unique<NfThumbnailTask>(photo, std::move(image));
        {
                std::scoped_lock lock(m_queueMutex);
                task->setGenerationId(m_generationId);
        }

        task->setResult([&](NfTask* result, NfTask::TaskStatus status) {
                if (status != NfTask::TaskStatus::Success)
                        return;

                auto* thumbnailTask = dynamic_cast<NfThumbnailTask*>(result);
                if (thumbnailTask) {
                        std::scoped_lock lock(m_queueMutex);

                        // Check if the thumbnail belongs to the current generation.
                        // If not, ignore it.
                        if (thumbnailTask->generationId() != m_generationId)
                                return;

                        auto thumbnail = thumbnailTask->takeThumbnail();
                        m_thumbnailsQueue.push_back(std::move(*thumbnail));
                }
        });

        m_threadPool.submit(std::move(task));
}

void NfPhotoLoader::requestPreview(const NfPhoto &photo, std::unique_ptr<NfImage> image)
{
        auto task = std::make_unique<NfPreviewTask>(photo, std::move(image));
        {
                std::scoped_lock lock(m_queueMutex);
                task->setGenerationId(m_generationId);
        }

        task->setResult([&](NfTask* result, NfTask::TaskStatus status) {
                if (status != NfTask::TaskStatus::Success)
                        return;

                auto* previewTask = dynamic_cast<NfPreviewTask*>(result);
                if (previewTask) {
                        std::scoped_lock lock(m_queueMutex);

                        // Check if the preview belongs to the current generation.
                        // If not, ignore it.
                        if (previewTask->generationId() != m_generationId)
                                return;

                        auto preview = previewTask->takePreview();
                        m_previewsQueue.push_back(std::move(*preview));
                }
        });

        m_threadPool.submit(std::move(task));
}

std::vector<NfPhoto> NfPhotoLoader::takePhotos()
{
        return m_pathScanner->takePhotos();
}

std::vector<NfThumbnail> NfPhotoLoader::takeThumbnails()
{
        std::scoped_lock lock(m_queueMutex);
        return std::move(m_thumbnailsQueue);
}

std::vector<NfPreview> NfPhotoLoader::takePreviews()
{
        std::scoped_lock lock(m_queueMutex);
        return std::move(m_previewsQueue);
}

} // namespace NfCore
