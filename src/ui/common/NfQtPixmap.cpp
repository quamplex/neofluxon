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
#include "core/NfImageData.h"

using namespace NfCore;

namespace NfDesktop {

void NfQtPixmap::setData(std::unique_ptr<NfImageData> data)
{
        //if (data->empty() || data->width() <= 0 || data->height() <= 0) {
        //        m_pixmapImage = QPixmap();
        //        return;
        //}

        /*QImage::Format fmt = QImage::Format_Invalid;

        switch (data->format()) {
        case NfImageData::ImageFormat::Format_RGB888:
                fmt = QImage::Format_RGB888;
                break;
        case NfImageData::ImageFormat::Format_RGBA8888:
                fmt = QImage::Format_RGBA8888;
                break;
        case NfImageData::ImageFormat::Format_ARGB32_Premultiplied:
                fmt = QImage::Format_ARGB32_Premultiplied;
                break;
        default:
                m_pixmapImage = QPixmap();
                return;
                }*/

        //const int bytesPerLine = data->width() * data->channels();
        QImage img;
        img.loadFromData(data->data(), data->size());
        //data->width(),
        //           data->height(),
        //           bytesPerLine,
        //           fmt);

        // Deep copy to detach from NfImageData memory
        m_pixmapImage = QPixmap::fromImage(img.copy());

        NfImage::setData(std::move(data));
}

const QPixmap& NfQtPixmap::pixmap() const
{
        return m_pixmapImage;
}

std::size_t NfQtPixmap::size() const
{
        if (m_pixmapImage.isNull())
                return 0;

        return static_cast<std::size_t>(m_pixmapImage.width()) *
                static_cast<std::size_t>(m_pixmapImage.height()) *
                static_cast<std::size_t>(m_pixmapImage.depth()) / 8;
}

} // namespace NfDesktop
