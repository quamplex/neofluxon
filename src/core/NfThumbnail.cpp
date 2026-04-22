/**
 * File name: NfThumbnail.cpp
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

#include "NfThumbnail.h"
#include "NfImage.h"

namespace NfCore {

NfThumbnail::NfThumbnail(const NfPhotoId &id, std::unique_ptr<NfImage> img)
        : m_photoId{id}
        , m_image{std::move(img)}
        , m_imageSource{ImageSource::EmbeddedImage}
{
}

void NfThumbnail::setImageSource(NfThumbnail::ImageSource source)
{
        m_imageSource = source;
}

NfThumbnail::ImageSource NfThumbnail::imageSource() const
{
        return m_imageSource;
}

const NfPhotoId& NfThumbnail::id() const
{
        return m_photoId;
}

NfImage* NfThumbnail::getImage() const
{
        return m_image.get();
}

std::unique_ptr<NfImage> NfThumbnail::releaseImage()
{
        return std::move(m_image);
}

} // namespace NfCore
