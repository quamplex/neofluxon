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
#include "NfGuiImage.h"
#include "NfLogger.h"

namespace NfCore {

NfPhotoLoader::NfPhotoLoader()
        : m_pathScanner{std::make_unique<NfPathScanner>()}
{
}

NfPhotoLoader::~NfPhotoLoader()
{
        NF_LOG_DEBUG("called");
}

void NfPhotoLoader::setPath(const std::filesystem::path &path)
{
        m_path = path;
        m_pathScanner->setPath(path);
}

const std::filesystem::path& NfPhotoLoader::getPath() const
{
        return m_path;
}

void NfPhotoLoader::requestThumbnail(const NfPhoto &photo,
                                     std::unique_ptr<NfGuiImage> imageContainer)
{
        auto task = std::make_unique<NfThumbnailTask>(photo, std::move(imageContainer));

        task->setResult([&](std::unique_ptr<NfTask> resultTask) {
                auto* thumbnailTask = dynamic_cast<NfThumbnailTask*>(resultTask.get());
                if (thumbnailTask) {
                        auto image = thumbnailTask->takeImage();
                        {
                                std::scoped_lock lock(m_thumbnailsQueueMutex);
                                m_thumbnailsQueue.push_back(std::move(image));
                        }
                }
        });

        m_threadPool->submit(std::move(task));
}

std::vector<NfPhoto> NfPhotoLoader::takePhotos()
{
        return m_pathScanner->takePhotos();
}

std::vector<NfGuiThumbnail> NfPhotoLoader::takeThumbnails()
{
        return {};
}

} // namespace NfCore
