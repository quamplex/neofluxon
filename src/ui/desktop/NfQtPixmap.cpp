/**
 * File name: NfQtPixmap.cpp
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

#include "NfQtPixmap.h"

void NfQtPixmap::setImageData(const NfImageData& imageData)
{
        if (imageData.empty() || imageData.width() <= 0 || imageData.height() <= 0) {
                m_pixmapImage = QPixmap();
                return;
        }

        auto fmt = QImage::Format_Invalid;
        auto imageFormat = imageData.format();

        switch (imageFormat) {
        case NfImageData::ImageFromat::Format_RGB888:
                fmt = QImage::Format_RGB888;
                break;
        case NfImageData::ImageFromat::Format_RGBA888:
                fmt = QImage::Format_RGBA8888;
                break;
        case NfImageData::ImageFromat::Format_ARGB32_Premultiplied:
                fmt = QImage::Format_ARGB32_Premultiplied;
                break;
        default:
                m_pixmapImage = QPixmap();
                return;
        }

        QImage img(imageData.data(), imageData.size(), fmt);
        m_pixmap = QPixmap::fromImage(img.copy());
}

const QPixmap& NfQtPixmap::pixmap() const
{
    return m_pixmapImage;
}
