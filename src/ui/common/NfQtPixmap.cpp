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
#include "core/NfLogger.h"

using namespace NfCore;

namespace NfUi {

void NfQtPixmap::setData(std::unique_ptr<NfImageData> data)
{
        m_pixmapImage = QPixmap();

        if (!data || data->empty() || data->width() <= 0 || data->height() <= 0)
                return;

        QImage img;
        const auto* rawPtr = reinterpret_cast<const uchar*>(data->data());
        bool isLoaded = false;

        if (data->format() == NfImageData::ImageFormat::Format_JPEG) {
                isLoaded = img.loadFromData(rawPtr, data->size());
        } else {
                auto qtFmt = QImage::Format_Invalid;
                switch (data->format()) {
                case NfImageData::ImageFormat::Format_RGB888:
                        qtFmt = QImage::Format_RGB888;
                        break;
                case NfImageData::ImageFormat::Format_RGBA8888:
                        qtFmt = QImage::Format_RGBA8888;
                        break;
                case NfImageData::ImageFormat::Format_ARGB32_Premultiplied:
                        qtFmt = QImage::Format_ARGB32_Premultiplied;
                        break;
                default:
                        return;
                }

                img = QImage(rawPtr, data->width(), data->height(), qtFmt);
                isLoaded = !img.isNull();
        }

        if (!isLoaded)
                return;

        if (data->orientation() > 0)
                fixOrientation(img, data->orientation());

        m_pixmapImage = QPixmap::fromImage(img.copy());
        NfImage::setData(std::move(data));
}

void NfQtPixmap::fixOrientation(QImage &img, int orientation)
{
        QTransform transform;

        switch (orientation) {
        case 1: // Vertical Flip
                transform.scale(1, -1);
                break;
        case 2: // Horizontal Flip
                transform.scale(-1, 1);
                break;
        case 3: // 180° Rotation
                transform.rotate(180);
                break;
        case 4: // 90° CW + Vertical Flip
                transform.rotate(90);
                transform.scale(1, -1);
                break;
        case 5: // 90° CCW (270° CW)
                transform.rotate(270);
                break;
        case 6: // 90° CW
                transform.rotate(90);
                break;
        case 7: // 90° CW + Horizontal Flip
                transform.rotate(90);
                transform.scale(-1, 1);
                break;
        default: // 0 is normal
                break;
        }

        if (!transform.isIdentity())
                img = img.transformed(transform, Qt::SmoothTransformation);
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

void NfQtPixmap::resize(int w, int h) override
{
        m_pixmapImage = m_pixmapImage.scaled(w, h, Qt::KeepAspectRatio);
        setWidth(m_pixmapImage.width());
        setHeight(m_pixmapImage.height());
}

} // namespace NfUi
