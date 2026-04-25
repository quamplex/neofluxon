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

namespace NfCore {

NfThumbnailTask::NfThumbnailTask(const NfPhoto& photo,
                                 std::unique_ptr<NfImage> imageContainer)
        : NfThumbnailTask(photo, std::move(imageContainer))
{
}

NfThumbnailTask::~NfThumbnailTask() = default;

NfThumbnailTask::TaskStatus NfThumbnailTask::execute()
{
        NfImageDecoder decoder(m_photo);
        std::unique_ptr<NfImageData> imageData;
        bool isRawData = false;

        const auto method = extractionMethod();

        if (method == ExtractionMethod::Embedded
            || method == ExtractionMethod::Fastest) {
                imageData = decoder.thumbnailImageData();
        }

        if (!imageData && (method == ExtractionMethod::FromRaw
                           || method == ExtractionMethod::Fastest)) {
                imageData = decoder.rawImage();
                isRawData = true;
        }

        if (!imageData)
                return TaskStatus::Failed;

        m_imageContainer->setData(std::move(imageData));
        if (isRawData)
                m_imageContainer->resize(256, 256);

        return TaskStatus::Success;
}

std::unique_ptr<NfThumbnail> NfThumbnailTask::takeThumbnail()
{
        auto thumb = std::make_unique<NfThumbnail>(m_photo.id(), std::move(m_imageContainer));
        thumb->setImageSource(imageSource());
        return thumb;
}

} // namespace NfCore
