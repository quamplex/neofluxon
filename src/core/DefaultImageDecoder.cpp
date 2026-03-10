/**
 * File name: DefaultImageDecoder.cpp
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

#include "DefaultImageDecoder.h"
#include "Application.h"

#include <QImage>

DefaultImageDecoder::DefaultImageDecoder(const QString &path)
        : ImageDecoder(path)
{
        //        LIQUIDFLOW_LOG_DEBUG() << path;
}

QImage DefaultImageDecoder::thumbnail() const
{
        //LIQUIDFLOW_LOG_DEBUG() << path();

        auto size = Liquid FlowApplication::getAppInstance()->thumbnailsSize();
        return QImage(path()).scaled(size,
                                    Qt::KeepAspectRatio/*,
                                                         Qt::SmoothTransformation*/);
}

QImage DefaultImageDecoder::image() const
{
        return QImage(path()).convertToFormat(QImage::Format_RGBA64);
}

std::unique_ptr<RawImageInfo> DefaultImageDecoder::loadImageInfo()
{
        return std::make_unique<RawImageInfo>();
}
