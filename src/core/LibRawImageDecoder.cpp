/**
 * File name: LibRawImage.cpp
 * Project: Liquid Flow (A photo management software)
 *
 * Copyright (C) 2023 Iurie Nistor
 *
 * This file is part of Liquid Flow.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#include "LibRawImageDecoder.h"

LibRawImageDecoder::LibRawImageDecoder(const QString &path)
        : ImageDecoder(path)
{
        //        LIQUIDFLOW_LOG_DEBUG() << path;
}

std::unique_ptr<LibRaw> LibRawImageDecoder::getProcessor() const
{
        auto processor = std::make_unique<LibRaw>();
        if (processor->open_file(path().toStdString().c_str()) != LIBRAW_SUCCESS)
                return nullptr;
        return processor;
}


QImage LibRawImageDecoder::thumbnail() const
{        
        auto rawProcessor = getProcessor();
        if (!rawProcessor)
                return QImage();

//        if (!getImageInfo())
        //               setImageInfo(loadRawInfo(rawProcessor));

        if (rawProcessor->unpack_thumb() != LIBRAW_SUCCESS)
                return QImage();

        int width = rawProcessor->imgdata.thumbnail.twidth;
        int height = rawProcessor->imgdata.thumbnail.theight;
        auto thumb = QImage(width, height, QImage::Format_RGB888);
        thumb.loadFromData(QByteArray(rawProcessor->imgdata.thumbnail.thumb,
                                      rawProcessor->imgdata.thumbnail.tlength));
        return thumb;
}

QImage LibRawImageDecoder::image() const
{
        auto rawProcessor = getProcessor();
        if (!rawProcessor)
                return QImage();

//        if (!getImageInfo())
//                setImageInfo(loadRawInfo(rawProcessor));

        if (rawProcessor->unpack() != LIBRAW_SUCCESS)
                return QImage();

        rawProcessor->imgdata.params.output_bps = 16;
        if (rawProcessor->dcraw_process() != LIBRAW_SUCCESS)
                return QImage();
        

        auto image = rawProcessor->dcraw_make_mem_image();
        if (!image)
                return QImage();

       QImage img(image->width, image->height, QImage::Format_RGBA64);

        auto alpha = std::numeric_limits<quint16>::max();
        auto data = reinterpret_cast<const quint16*>(image->data);
        size_t dataSize = image->data_size / (image->bits / 8);
        auto imgData = reinterpret_cast<quint16*>(img.bits());
        size_t bytesPerPixel = image->colors * (image->bits / 8);
        for (size_t k = 0, i = 0; i < dataSize; i += image->colors ) {
                std::memcpy(imgData + k, data + i, bytesPerPixel);
                imgData[k + 3] = alpha;
                k += 4;
        }

        LibRaw::dcraw_clear_mem(reinterpret_cast<decltype(image)>(image));
        rawProcessor->recycle();
        
        return img;
}

std::unique_ptr<RawImageInfo> LibRawImageDecoder::loadImageInfo()
{
        auto processor = getProcessor();
        if (!processor)
                return std::make_unique<RawImageInfo>();
        return loadRawInfo(processor);
}

std::unique_ptr<RawImageInfo> LibRawImageDecoder::loadRawInfo(const std::unique_ptr<LibRaw> &processor)
{
        auto info = std::make_unique<RawImageInfo>();
        info->setISO(processor->imgdata.other.iso_speed);
        info->setShutter(processor->imgdata.other.shutter);
        info->setAperture(processor->imgdata.other.aperture);
        info->setLensFocalLengh(processor->imgdata.other.focal_len);
        
        auto seconds = std::chrono::seconds(processor->imgdata.other.timestamp);
        auto duration = std::chrono::duration_cast<std::filesystem::file_time_type::duration>(seconds);
        info->setTakenDate(std::filesystem::file_time_type(duration));
        
        return info;
}


