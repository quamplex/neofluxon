/**
 * File name: ImageData.h
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

#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include "Liquid Flow.h"
#include "ImageDecoder.h"

#include <QUuid>

class ImageData
{
 public:
        ImageData(const QString &path);
        const QString& getUniqueId() const;
        const QString& path() const;
        const QString& getFileName() const;
        QImage thumbnail() const;
        bool createImagePreview() const;
        QImage imagePreview() const;
        QImage image() const;
        double getISO() const;
        double getAperture() const;
        double getShutter() const;
        double getLensFocalLength() const;
        const QString& getLensName() const;
        QString getExposureInfo() const;
        const std::filesystem::file_time_type& takenDate() const;

protected:
        QString getPreviewPath() const;
        
 private:
        QString photoUniqueId;
        QString fileName;
        std::unique_ptr<ImageDecoder> imageDecoder;
};

#endif // IMAGE_DATA_H
