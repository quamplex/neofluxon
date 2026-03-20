/**
 * File name: NfHash.cpp
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

#include "NfHash.h"

namespace NfCore {

NfHash::NfHash()
        : m_type(HashType::DataHash)
        , m_algorithm(HashAlgorithm::SHA256)
{
}

NfHash::NfHash(HashType type,
               HashAlgorithm algorithm,
               std::vector<uint8_t> value)
        : m_type{type}
        , m_algorithm{algorithm}
        , m_value(std::move(value))
{
}

NfHash NfHash::fromData([[maybe_unused]] const uint8_t* data,
                        [[maybe_unused]] size_t size,
                        [[maybe_unused]] HashType type,
                        [[maybe_unused]] HashAlgorithm algorithm)
{
    // TODO: implement hashing from raw data
        return {};
}

NfHash NfHash::fromFile([[maybe_unused]] const std::filesystem::path& path,
                        [[maybe_unused]] HashAlgorithm algorithm)
{
        // TODO: implement file hashing (streaming)
        return {};
}

NfHash NfHash::fromMetadata([[maybe_unused]] const std::filesystem::path& path,
                            [[maybe_unused]] HashAlgorithm algorithm)
{
        // TODO: implement file metadata hashing (streaming)
        return {};
}

bool NfHash::operator==(const NfHash& other) const
{
        return m_type == other.m_type &&
                m_algorithm == other.m_algorithm &&
                m_value == other.m_value;
}

bool NfHash::operator!=(const NfHash& other) const
{
        return !(*this == other);
}

NfHash::HashType NfHash::type() const
{
        return m_type;
}

NfHash::HashAlgorithm NfHash::algorithm() const
{
        return m_algorithm;
}

bool NfHash::isValid() const
{
        return !m_value.empty();
}

} // namespace NfCore
