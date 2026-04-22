/**
 * File name: NfPreviewTask.cpp
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

#include "NfPreviewTask.h"
#include "NfImageDecoder.h"
#include "NfImageData.h"
#include "NfImage.h"
#include "NfPreview.h"

#include <stdexcept>
#include <iostream>

namespace NfCore {

NfPreviewTask::NfPreviewTask(const NfPhoto& photo,
                                 std::unique_ptr<NfImage> imageContainer)
        : m_generationId{0}
        , m_photo{photo}
        , m_imageContainer{std::move(imageContainer)}
        , m_imageSource{ImageSource::EmbeddedImage}
{
}

NfPreviewTask::~NfPreviewTask() = default;

void NfPreviewTask::setGenerationId(uint64_t generationId)
{
        m_generationId = generationId;
}

uint64_t NfPreviewTask::generationId() const
{
        return m_generationId;
}

void NfPreviewTask::setImageSource(NfPreviewTask::ImageSource source)
{
        m_imageSource = source;
}

NfPreviewTask::ImageSource NfPreviewTask::imageSource() const
{
        return m_imageSource;
}

NfPreviewTask::TaskStatus NfPreviewTask::execute()
{
        NfImageDecoder decoder(m_photo);
        auto image = decoder.previewImageData();
        if (!image)
                return TaskStatus::Failed;

        m_imageContainer->setData(std::move(image));
        return TaskStatus::Success;
}

std::unique_ptr<NfPreview> NfPreviewTask::takePreview()
{
        auto preview = std::make_unique<NfPreview>(m_photo.id(),
                                                   std::move(m_imageContainer));
        preview->setImageSource(imageSource());
        return preview;
}

} // namespace NfCore
