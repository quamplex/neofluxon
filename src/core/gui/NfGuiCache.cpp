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

NfGuiCache::NfGuiCache(IDiskCache* diskCache, std::size_t maxSizeBytes)
        : m_diskCache{diskCache}
        , m_maxSizeBytes{maxSizeBytes}
        , m_currentSizeBytes{0}
{
}

void NfGuiCache::add(NfPhotoId id, std::unique_ptr<NfGuiImage> image)
{
        if (!image)
                return;

        std::size_t imageSizeBytes = image->size();

        // Reject oversized images that can never fit
        if (imageSizeBytes > m_maxSizeBytes)
                return;

        // Check if already exists - update and move to front
        if (auto it = m_memoryCache.find(id); it != m_memoryCache.end()) {
                std::size_t oldSizeBytes = it->second->size();
                it->second = std::move(image);
                m_currentSizeBytes -= oldSizeBytes;
                m_currentSizeBytes += imageSizeBytes;
                refreshAccess(id);
                return;
        }

        // Evict older items if we don't have enough space
        if (m_currentSizeBytes + imageSizeBytes > m_maxSizeBytes)
                evictUntilFits(imageSizeBytes);

        m_memoryCache.emplace(std::move(id), std::move(image));
        m_currentSizeBytes += imageSizeBytes;
        m_lruOrder.push_front(id);
}

std::optional<NfGuiImage*> NfGuiCache::get(const NfPhotoId& id)
{
        auto it = m_memoryCache.find(id);
        if (it == m_memoryCache.end())
                return std::nullopt;

        refreshAccess(id);
        return &it->second;
}

bool NfGuiCache::remove(const NfPhotoId& id)
{
        auto it = m_memoryCache.find(id);
        if (it == m_memoryCache.end())
                return false;

        m_currentSizeBytes -= it->second.size();
        m_memoryCache.erase(it);
        removeFromLRU(id);

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

void NfGuiCache::refreshAccess(const NfPhotoId& id)
{
        m_lruOrder.remove(id);
        m_lruOrder.push_front(id);
}

void NfGuiCache::removeFromLRU(const NfPhotoId& id)
{
        m_lruOrder.remove(id);
}

void NfGuiCache::evictUntilFits(std::size_t requiredSize)
{
        // Evict LRU items until we have enough space for the new image
        while (!m_lruOrder.empty() && (m_currentSizeBytes + requiredSize > m_maxSizeBytes)) {
                const auto& oldestId = m_lruOrder.back();

                auto it = m_memoryCache.find(oldestId);
                if (it != m_memoryCache.end()) {
                        std::size_t evictedSize = it->second->size();

                        // Save to disk if available, otherwise discard
                        if (m_diskCache)
                                m_diskCache->add(oldestId, std::move(it->second));

                        m_memoryCache.erase(it);
                        m_currentSizeBytes -= evictedSize;
                }

                m_lruOrder.pop_back();
        }
}
