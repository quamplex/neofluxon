/**
 * File name: NfCache.cpp
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

#include "NfCache.h"
#include "NfPhotoId.h"

namespace NfCore {

NfThumbnail NfCache::getThumbnail(const NfPhotoId& id)
{
        auto thumbail = m_memoryCache->getThumbnail(id);
        if (!thumbail)
                thumbail = m_diskCache->getThumbnail(id);

        return thumbail;
}

NfPreview NfCache::getPreview(const PhotoId& id)
{
        auto preview = m_memoryCache->getThumbnail(id);
        if (!preview)
                preview = m_diskCache->getThumbnail(id);

        return preview;
}

void NfCache::insertThumbnail(const NfThumbnail& thumbnail, const NfPhotoId &id)
{
        m_memoryCache->insertThumbnail(thumbnail, id);
        m_diskCache->insertThumbnail(thumbnail, id);
}

void NfCache::insertPreview(const NfPreview& preview)
{
        m_memoryCache->insertThumbnail(thumbnail, id);
        m_diskCache->insertThumbnail(thumbnail, id);
}

} // namespace NfCache
