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

namespace NfCore {

NfImageData::NfImageData()
        : m_width{0}
        , m_height{0}
        , m_channels{0}
{
}

NfImageData::NfImageData(size_t size)
        : m_data(size)
        , m_width{0}
        , m_height{0}
        , m_channels{0}
{
}

NfImageData::NfImageData(int w, int h, int c)
        : m_data(static_cast<size_t>(w) * static_cast<size_t>(h) * static_cast<size_t>(c))
        , m_width{w}
        , m_height{h}
        , m_channels{c}

{
}

NfImageData::~NfImageData()
{
}

void NfImageData::setData(const unsigned char* ptr, size_t len)
{
        m_data.assign(ptr, ptr + len);
}

const unsigned char* NfImageData::data() const
{
        return m_data.data();
}

unsigned char* NfImageData::data()
{
        return m_data.data();
}

void NfImageData::resize(size_t newSize)
{
        m_data.resize(newSize);
}

size_t NfImageData::size() const
{
        return m_data.size();
}

void NfImageData::clear()
{
        m_data.clear();
}

bool NfImageData::empty() const
{
        return m_data.empty();
}

void NfImageData::setWidth(int w)
{
        m_width = w;
}

int NfImageData::width() const
{
        return m_width;
}

void NfImageData::setHeight(int h)
{
        m_height = h;
}

int NfImageData::height() const
{
        return m_height;
}

void NfImageData::setChannels(int c)
{
        m_channels;
}

int NfImageData::channels() const
{
        return m_channels;
}

NfImageData::ImageFormat NfImageData::format() const
{
        return ImageFormat::Format_ARGB32_Premultiplied;
};

} // namespace NfCore
