/**
 * File name: NfImageData.cpp
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

#include "NfImageData.h"

#include <cstring>

NfImageData::NfImageData()
        : m_width{0}
        , m_height{0}
        , m_channels{0}
{
}

NfImageData::NfImageData(int w, int h, int c)
        : m_width{w}
        , m_height{h}
        , m_channels{c}
{
    pixels.resize(static_cast<size_t>(w) * h * c);
}

const unsigned char* NfImageData::data() const
{
    return pixels.data();
}

unsigned char* NfImageData::data()
{
    return pixels.data();
}

size_t NfImageData::size() const
{
    return pixels.size();
}

bool NfImageData::empty() const
{
    return pixels.empty() || width <= 0 || height <= 0 || channels <= 0;
}

void NfImageData::setData(const void* ptr, size_t len)
{
    if (!ptr || len == 0)
    {
        pixels.clear();
        width = height = channels = 0;
        return;
    }

    pixels.resize(len);
    std::memcpy(pixels.data(), ptr, len);
    // TODO: width/height/channels must be set separately if needed
}
