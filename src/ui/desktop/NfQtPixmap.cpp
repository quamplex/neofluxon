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

void NfQtPixmap::loadData(const NfImageData& imageData)
{
    if (imageData.pixels.empty() || imageData.width <= 0 || imageData.height <= 0)
    {
        m_pixmap = QPixmap();
        return;
    }

    // Determine format based on channels
    QImage::Format fmt = QImage::Format_Invalid;
    if (imageData.channels == 3)
        fmt = QImage::Format_RGB888;
    else if (imageData.channels == 4)
        fmt = QImage::Format_RGBA8888;

    if (fmt == QImage::Format_Invalid)
    {
        m_pixmap = QPixmap();
        return;
    }

    // Construct QImage from raw data (copy)
    QImage img(imageData.data(), imageData.size(), fmt);

    // Make a deep copy to own the data
    m_pixmap = QPixmap::fromImage(img.copy());
}

NfImageData NfQtPixmap::getImageData() const
{
    NfImageData out;

    if (m_pixmap.isNull())
        return out;

    // Convert QPixmap -> QImage
    QImage img = m_pixmap.toImage();

    // Convert to known format (RGB888)
    img = img.convertToFormat(QImage::Format_RGB888);

    out.width = img.width();
    out.height = img.height();
    out.channels = 3;

    // Copy raw pixel data
    out.pixels.resize(out.width * out.height * out.channels);
    std::memcpy(out.pixels.data(), img.bits(), out.pixels.size());

    return out;
}

const QPixmap& NfQtPixmap::pixmap() const
{
    return m_pixmap;
}
