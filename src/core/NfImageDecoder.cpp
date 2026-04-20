/**
 * File name: NfImageDecoder.cpp
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

#include "NfImageDecoder.h"
#include "NfImageData.h"
#include "NfImage.h"
#include "NfLogger.h"

#include "libraw/libraw.h"

namespace NfCore {

NfImageDecoder::NfImageDecoder(const NfPhoto &photo)
        : m_photo{photo}
{
}

NfImageDecoder::~NfImageDecoder() = default;

std::unique_ptr<NfImageData> NfImageDecoder::thumbnailImageData() const
{
        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(m_photo.path().string().c_str()) != LIBRAW_SUCCESS) {
                NF_LOG_DEBUG("can't open file : " << m_photo.path());
                return nullptr;
        }

        auto &thumbList = rawProcessor->imgdata.thumbs_list;
        int count = thumbList.thumbcount;
        NF_LOG_DEBUG("thumbList.thumbcount: " << count);

        if (count == 0)
                return nullptr;

        // TODO
        if (rawProcessor->unpack_thumb_ex(0) != LIBRAW_SUCCESS)
                return nullptr;

        NF_LOG_DEBUG("thumbnail unpack: OK");

        auto &t = rawProcessor->imgdata.thumbnail;

        if (t.tformat != LIBRAW_THUMBNAIL_JPEG || t.tlength == 0)
                return nullptr;

        auto imageData = std::make_unique<NfImageData>();
        imageData->setData(reinterpret_cast<const unsigned char*>(t.thumb), t.tlength);
        imageData->setOrientation(rawProcessor->imgdata.sizes.flip);

        return imageData;
}

std::unique_ptr<NfImageData> NfImageDecoder::previewImageData() const
{
        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(m_photo.path().string().c_str()) != LIBRAW_SUCCESS)
                return nullptr;

        auto &thumbList = rawProcessor->imgdata.thumbs_list;
        int count = thumbList.thumbcount;

        if (count == 0)
                return nullptr;

        // Find the best preview (usually the largest/last one)
        // Often, index 0 is a tiny thumb, and higher indices are previews.
        int bestIndex = 0;
        int maxWidth = 0;
        for (int i = 0; i < count; ++i) {
                if (thumbList.thumblist[i].twidth > maxWidth) {
                        maxWidth = thumbList.thumblist[i].twidth;
                        bestIndex = i;
                }
        }

        // Unpack the specific index we found
        if (rawProcessor->unpack_thumb_ex(bestIndex) != LIBRAW_SUCCESS) {
                return nullptr;
        }

        auto &t = rawProcessor->imgdata.thumbnail;
        if (t.tformat != LIBRAW_THUMBNAIL_JPEG || t.tlength == 0)
                return nullptr;

        auto imageData = std::make_unique<NfImageData>();
        imageData->setData(reinterpret_cast<const unsigned char*>(t.thumb), t.tlength);
        imageData->setOrientation(rawProcessor->imgdata.sizes.flip);

        return imageData;
}

} // namespace NfCore

