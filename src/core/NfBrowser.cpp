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

namespace NfCore {

NfBrowser::NfBrowser()
{
};

NfBrowser::~NfBrowser()
{
}

void NfBrowser::setThumbnailReadyCallback(ThumbnailReadyCallback callback)
{
        m_thumbnailReadyCallback = std::move(callback);
}

NfThumbnail NfBrowser::getThumbnailAt(size_t index) const
{
        m_photoCache->getThumbnail();
        if ()....
        return {};
}

size_t NfBrowser::numberOfThumbnails() const
{
        return 0;
}

} // namespace NfCore
