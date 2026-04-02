/**
 * File name: NfGuiCache.cpp
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

#include "NfGuiCache.h"
//#include "NfDiskCache.h"
#include "NfGuiImage.h"

namespace NfCore {

NfGuiCache::NfGuiCache(/*NfDiskCache* diskCache, */std::size_t maxSizeBytes)
//        : m_diskCache{diskCache}
        : m_maxSizeBytes{maxSizeBytes}
        , m_currentSizeBytes{0}
{
}

NfGuiCache::~NfGuiCache() = default;

void NfGuiCache::add(const NfPhotoId &id, std::unique_ptr<NfGuiImage> image)
{
        if (!image)
                return;

        std::size_t imageSizeBytes = image->size();

        // Reject oversized images that can never fit
        if (imageSizeBytes > m_maxSizeBytes)
                return;

        // Check if already exists - update and move to front
        if (auto it = m_memoryCache.find(id); it != m_memoryCache.end()) {
                auto& cacheImage = it->second.second;
                std::size_t oldSizeBytes = cacheImage->size();
                cacheImage = std::move(image);
                m_currentSizeBytes -= oldSizeBytes;
                m_currentSizeBytes += imageSizeBytes;
                refreshAccess(it->second.first);
                return;
        }

        // Evict older items if we don't have enough space
        if (m_currentSizeBytes + imageSizeBytes > m_maxSizeBytes)
                evictUntilFits(imageSizeBytes);

        m_lruOrder.push_front(id);
        m_memoryCache.emplace(id, std::make_pair(m_lruOrder.begin(), std::move(image)));
        m_currentSizeBytes += imageSizeBytes;
}

NfGuiImage* NfGuiCache::get(const NfPhotoId& id)
{
        auto it = m_memoryCache.find(id);
        if (it == m_memoryCache.end())
                return nullptr;

        refreshAccess(it->second.first);

        return it->second.second.get();
}

bool NfGuiCache::remove(const NfPhotoId& id)
{
        auto it = m_memoryCache.find(id);
        if (it == m_memoryCache.end())
                return false;

        m_currentSizeBytes -= it->second.second->size();
        removeFromLRU(it->second.first);
        m_memoryCache.erase(it);

        return true;
}

void NfGuiCache::clear()
{
        m_memoryCache.clear();
        m_lruOrder.clear();
        m_currentSizeBytes = 0;
}

std::size_t NfGuiCache::currentSizeBytes() const noexcept
{
        return m_currentSizeBytes;
}

std::size_t NfGuiCache::maxSizeBytes() const noexcept
{
        return m_maxSizeBytes;
}

void NfGuiCache::setMaxSizeBytes(std::size_t maxSizeBytes)
{
        m_maxSizeBytes = maxSizeBytes;
        if (m_currentSizeBytes > m_maxSizeBytes)
                evictUntilFits(0);
}

void NfGuiCache::refreshAccess(std::list<NfPhotoId>::iterator it)
{
        m_lruOrder.splice(m_lruOrder.begin(), m_lruOrder, it);
}

void NfGuiCache::removeFromLRU(std::list<NfPhotoId>::iterator it)
{
        m_lruOrder.erase(it);
}

void NfGuiCache::evictUntilFits(std::size_t requiredSize)
{
        // Evict LRU items until we have enough space for the new image
        while (!m_lruOrder.empty() && (m_currentSizeBytes + requiredSize > m_maxSizeBytes)) {
                const auto& oldestId = m_lruOrder.back();

                auto it = m_memoryCache.find(oldestId);
                if (it != m_memoryCache.end()) {
                        std::size_t evictedSize = it->second.second->size();

                        // Save to disk if available, otherwise discard
                        //if (m_diskCache)
                        //        m_diskCache->add(oldestId, std::move(it->second));

                        m_memoryCache.erase(it);
                        m_currentSizeBytes -= evictedSize;
                }

                m_lruOrder.pop_back();
        }
}

} // namespace NfCore
