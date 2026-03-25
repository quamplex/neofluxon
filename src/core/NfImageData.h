/**
 * File name: NfImageData.h
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

#ifndef NF_IMAGE_DATA_H
#define NF_IMAGE_DATA_H

#include "Nefluxon.h"

#include <cstddef>

struct NfImageData
{
    std::vector<unsigned char> pixels; // raw pixel data
    int width;
    int height;
    int channels; // 3 = RGB, 4 = RGBA

    NfImageData();
    NfImageData(int w, int h, int c);

    // Accessors
    const unsigned char* data() const;
    unsigned char* data();
    size_t size() const;
    bool empty() const;

    // Set data from raw memory
    void setData(const void* ptr, size_t len);
};

#endif // NF_IMAGE_DATA_H
