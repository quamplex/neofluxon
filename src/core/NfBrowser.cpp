/**
 * File name: NfBrowser.cpp
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

#include "NfBrowser.h"
#include "ForegroundThreadPool.h"
#include "NfPathLoader.h"

namespace NfCore {

NfBrowser::NfBrowser(NfCache *cache)
        : m_cache{cache}
        , m_photoLoader{std::make_unique<NfPathLoader>()}
{
        
}

NfBrowser::~NfBrowser()
{
}

void NfBrowser::setItemNumberChangedCallback(ItemNumberChangedCallback callback)
{
        m_itemNumberChangedCallback = std::move(callback);
}

void NfBrowser::setThumbnailReadyCallback(ThumbnailReadyCallback callback)
{
        m_thumbnailReadyCallback = std::move(callback);
}

void NfBrowser::setPath(const std::filesystem::path &path)
{
        m_pathLoader->setPath(path);
}

std::filesystem::path NfBrowser::getPath() const
{
        return m_pathLoader->getPath(path);
}

NfThumbnail NfBrowser::getThumbnailAt(size_t index) const
{
        auto photoId = m_pathLoader->getId(index);
        if (!photoId) {
                m_thumbnailReadyCallback
                return {};
        }

        auto thumbail = m_cache->getThumbnail(photoId);
        if (!thumbail) {
                m_photoProvider->requestThumbail(photoId);
                return {};
        }

        return thumbail;
}

size_t NfBrowser::numberOfThumbnails() const
{
        return m_pathLoader->numberOfFiles();
}

} // namespace NfCore
