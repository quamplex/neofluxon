/**
 * File name: NfPhotoProvider.cpp
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

#include "NfPhotoProvider.h"
#include "ForegroundThreadPool.h"
#include "NfPathLoader.h"

namespace NfCore {

NfPhotoProvider::NfPhotoProvider(NfCache *cache)
        : m_cache{cache}
        , m_photoLoader{std::make_unique<NfPathLoader>()}
{
        
}

NfPhotoProvider::~NfPhotoProvider()
{
}

void NfPhotoProvider::setItemNumberChangedCallback(ItemNumberChangedCallback callback)
{
        m_itemNumberChangedCallback = std::move(callback);
}

void NfPhotoProvider::setThumbnailReadyCallback(ThumbnailReadyCallback callback)
{
        m_thumbnailReadyCallback = std::move(callback);
}

void NfPhotoProvider::requestThumbnail(const NfPhotoInfo &info,
                                       std::unique_ptr<NfImage> imageContainer)
{
    {
        std::scoped_lock lock(m_queueMutex);
        m_thubnailsQueue.push(std::move(info));
    }

    m_cv.notify_one();
}

void NfPhotoProvider::generateThumbnail(std::unique_ptr<NfImage> image)
{
        // Ge the thubnail data

        image->setData(imageData);
        m_thumbnailReadyCallback(std::move());
}

void NfPhotoProvider::setPath(const std::filesystem::path &path)
{
        m_pathLoader->setPath(path);
}

std::filesystem::path NfPhotoProvider::getPath() const
{
        return m_pathLoader->getPath(path);
}

} // namespace NfCore
