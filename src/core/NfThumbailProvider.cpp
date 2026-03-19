/**
 * File name: NfThumbnailProvider.cpp
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

#include "NfThumbnailProvider.h"
#include "ForegroundThreadPool.h"

namespace NfCore {

NfThumbnailProvider::NfThumbnailProvider()
{
};

NfThumbnailProvider::~NfThumbnailProvider()
{
}

void NfThumbnailProvider::setThumbnailReadyCallback(ThumbnailReadyCallback callback)
{
        m_thumbnailReadyCallback = std::move(callback);
}

NfThumbnail NfThumbnailProvider::getThumbnailAt(size_t index) const
{
        return {};
}

size_t NfThumbnailProvider::numberOfThumbnails() const
{
        return 0;
}

void NfThumbnailProvider::requestThumbnails(size_t startIndex, size_t count)
{
}

void NfThumbnailProvider::generateThumbnail(size_t index)
{

}

} // namespace NfCore
