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

#include <libraw/libraw.h>

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

        int bestIndex = selectThumbnail(rawProcessor->imgdata.thumbs_list);

        if (bestIndex < 0) {
                // TODO: generate thumbnail from the raw image.
                NF_LOG_ERROR("no thumbnail");
                return nullptr;
        }

        NF_LOG_ERROR("unpack thumbnail at index: " << bestIndex);
        if (rawProcessor->unpack_thumb_ex(bestIndex) != LIBRAW_SUCCESS) {
                NF_LOG_ERROR("can't unpack thumbnail at index: " << bestIndex);
                return nullptr;
        }

        NF_LOG_DEBUG("thumbnail unpack: OK");

        auto &t = rawProcessor->imgdata.thumbnail;

        if (t.tformat != LIBRAW_THUMBNAIL_JPEG || t.tlength == 0) {
                NF_LOG_ERROR("thumbnail is not JPEG");
                // TODO: generate thumbnail from the raw image.
                return nullptr;
        }

        auto imageData = std::make_unique<NfImageData>();
        imageData->setData(reinterpret_cast<const unsigned char*>(t.thumb), t.tlength);
        imageData->setOrientation(rawProcessor->imgdata.sizes.flip);

        return imageData;
}

std::unique_ptr<NfImageData> NfImageDecoder::previewImageData() const
{
        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(m_photo.path().string().c_str()) != LIBRAW_SUCCESS) {
                NF_LOG_ERROR("can't open file: " << m_photo.path());
                return nullptr;
        }

        auto bestIndex = selectPreview(rawProcessor->imgdata.thumbs_list);
        if (bestIndex < 0) {
                NF_LOG_ERROR("can't find preview");
                // TODO: generate preview from the raw image.
                return nullptr;
        }

        NF_LOG_ERROR("unpack preview at index: " << bestIndex);
        if (rawProcessor->unpack_thumb_ex(bestIndex) != LIBRAW_SUCCESS) {
                NF_LOG_ERROR("can't unpack the preview image");
                return nullptr;
        }

        NF_LOG_DEBUG("preview unpack: OK");

        auto &t = rawProcessor->imgdata.thumbnail;
        if (t.tformat != LIBRAW_THUMBNAIL_JPEG || t.tlength == 0) {
                NF_LOG_ERROR("preview image is not JPEG");
                // TODO: generate preview from the raw image.
                return nullptr;
        }

        auto imageData = std::make_unique<NfImageData>();
        imageData->setData(reinterpret_cast<const unsigned char*>(t.thumb), t.tlength);
        imageData->setOrientation(rawProcessor->imgdata.sizes.flip);

        return imageData;
}

int NfImageDecoder::selectThumbnail(const libraw_thumbnail_list_t& list)
{
        return selectBestForTarget(list, 256);
}

int NfImageDecoder::selectPreview(const libraw_thumbnail_list_t& list)
{
        return selectBestForTarget(list, 1600);
}

int NfImageDecoder::selectBestForTarget(const libraw_thumbnail_list_t& list,
                                        int targetSize)
{
        int bestIndex = -1;
        int minDistance = INT_MAX;

        NF_LOG_DEBUG("target: " << targetSize);

        for (int i = 0; i < list.thumbcount; i++) {
                int w = list.thumblist[i].twidth;
                int h = list.thumblist[i].theight;

                NF_LOG_DEBUG("[" << i << "][" << w << "x" << h << "]");

                if (w <= 0 || h <= 0)
                        continue;

                int size = std::max(w, h);
                int distance = std::abs(size - targetSize);

                if (distance < minDistance) {
                        minDistance = distance;
                        bestIndex = i;
                }
        }

        NF_LOG_DEBUG("Best index:" << bestIndex);

    return bestIndex;
}

} // namespace NfCore

