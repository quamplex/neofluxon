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
        NF_LOG_DEBUG("open file: " << m_photo.path());

        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(m_photo.path().string().c_str()) != LIBRAW_SUCCESS) {
                NF_LOG_DEBUG("can't open file : " << m_photo.path());
                return nullptr;
        }

        NF_LOG_DEBUG("open file OK");
        NF_LOG_DEBUG("original size:"
                     << rawProcessor->imgdata.sizes.width
                     << "x" << rawProcessor->imgdata.sizes.height);

        int bestIndex = selectThumbnail(rawProcessor->imgdata.thumbs_list);
        if (bestIndex < 0) {
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

        NF_LOG_DEBUG("thumbnail format: " << t.tformat);

        if (!isSupportedFormat(t.tformat)) {
                NF_LOG_ERROR("format " << t.tformat << " not supported");
                return nullptr;
        }

        auto imageData = std::make_unique<NfImageData>();
        imageData->setData(reinterpret_cast<const unsigned char*>(t.thumb), t.tlength);
        imageData->setFormat(libRawToNfImageFormat(t.tformat));
        imageData->setOrientation(rawProcessor->imgdata.sizes.flip);
        imageData->setWidth(t.twidth);
        imageData->setHeight(t.theight);

        NF_LOG_DEBUG("thumbnail loaded:  " << m_photo.path());
        NF_LOG_DEBUG("format: " << static_cast<int>(imageData->format()));
        NF_LOG_DEBUG("dimentions: " << imageData->width() << "x" << imageData->height());

        return imageData;
}

std::unique_ptr<NfImageData> NfImageDecoder::previewImageData() const
{
        NF_LOG_DEBUG("open file: " << m_photo.path());

        auto rawProcessor = std::make_unique<LibRaw>();
        if (rawProcessor->open_file(m_photo.path().string().c_str()) != LIBRAW_SUCCESS) {
                NF_LOG_ERROR("can't open file: " << m_photo.path());
                return nullptr;
        }

        NF_LOG_DEBUG("open file OK");
        NF_LOG_DEBUG("original size:"
                     << rawProcessor->imgdata.sizes.width
                     << "x" << rawProcessor->imgdata.sizes.height);

        auto bestIndex = selectPreview(rawProcessor->imgdata.thumbs_list);
        if (bestIndex < 0) {
                NF_LOG_ERROR("can't find preview");
                return nullptr;
        }

        NF_LOG_DEBUG("unpack preview at index: " << bestIndex);

        if (rawProcessor->unpack_thumb_ex(bestIndex) != LIBRAW_SUCCESS) {
                NF_LOG_ERROR("can't unpack the preview image");
                return nullptr;
        }

        NF_LOG_DEBUG("preview unpack: OK");

        auto &t = rawProcessor->imgdata.thumbnail;

        NF_LOG_ERROR("format: " << t.tformat);

        if (!isSupportedFormat(t.tformat)) {
                NF_LOG_ERROR("format " << t.tformat << " not supported");
                return nullptr;
        }

        auto imageData = std::make_unique<NfImageData>();
        imageData->setData(reinterpret_cast<const unsigned char*>(t.thumb), t.tlength);
        imageData->setFormat(libRawToNfImageFormat(t.tformat));
        imageData->setOrientation(rawProcessor->imgdata.sizes.flip);
        imageData->setWidth(t.twidth);
        imageData->setHeight(t.theight);

        NF_LOG_DEBUG("preview loaded:  " << m_photo.path());
        NF_LOG_DEBUG("format: " << static_cast<int>(imageData->format()));
        NF_LOG_DEBUG("dimentions: " << imageData->width() << "x" << imageData->height());

        return imageData;
}

NfImageData::ImageFormat NfImageDecoder::libRawToNfImageFormat(int format)
{
        switch (format) {
        case LIBRAW_THUMBNAIL_JPEG:
                return NfImageData::ImageFormat::Format_JPEG;
        case LIBRAW_THUMBNAIL_BITMAP:
                return NfImageData::ImageFormat::Format_RGB888;
        default:
                return NfImageData::ImageFormat::Format_Unknown;
        }
}

bool NfImageDecoder::isSupportedFormat(int format)
{
        return (format == LIBRAW_THUMBNAIL_JPEG)
                || (format == LIBRAW_THUMBNAIL_BITMAP);
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

