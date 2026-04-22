/**
 * File name: NfThumbnailTask.cpp
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

#include "NfThumbnailTask.h"
#include "NfImageDecoder.h"
#include "NfImageData.h"
#include "NfImage.h"
#include "NfThumbnail.h"

#include <stdexcept>
#include <iostream>

namespace NfCore {

NfThumbnailTask::NfThumbnailTask(const NfPhoto& photo,
                                 std::unique_ptr<NfImage> imageContainer)
        : m_generationId{0}
        , m_photo{photo}
        , m_imageContainer{std::move(imageContainer)}
        , m_imageSource{ImageSource::EmbeddedImage}
{
}

NfThumbnailTask::~NfThumbnailTask() = default;

void NfThumbnailTask::setGenerationId(uint64_t generationId)
{
        m_generationId = generationId;
}

uint64_t NfThumbnailTask::generationId() const
{
        return m_generationId;
}

void NfThumbnailTask::setImageSource(ImageSource source)
{
        m_imageSource = source;
}

ImageSource NfThumbnailTask::imageSource() const
{
        return m_imageSource;
}

NfThumbnailTask::TaskStatus NfThumbnailTask::execute()
{
        NfImageDecoder decoder(m_photo);
        auto imageData = decoder.thumbnailImageData();
        if (!imageData)
                return TaskStatus::Failed;

        m_imageContainer->setData(std::move(imageData));
        return TaskStatus::Success;
}

std::unique_ptr<NfThumbnail> NfThumbnailTask::takeThumbnail()
{
        return std::make_unique<NfThumbnail>(m_photo.id(), std::move(m_imageContainer));
}

} // namespace NfCore
