/**
 * File name: NfHash.h
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

#ifndef NF_HASH_H
#define NF_HASH_H

#include "Neofluxon.h"

namespace NfCore {

class NfHash {
public:
        enum class HashType {
                DataHash,
                FileHash,
                MetadataHash
        };

        enum class HashAlgorithm {
                SHA256,
                BLAKE3,
                XXHash
        };

        NfHash();

        NfHash(HashType type,
               HashAlgorithm algorithm,
               std::vector<uint8_t> value);

        static NfHash fromData(const uint8_t* data,
                               size_t size,
                               HashType type = HashType::DataHash,
                               HashAlgorithm algorithm = HashAlgorithm::SHA256);
        static NfHash fromFile(const std::filesystem::path& path,
                               HashAlgorithm algorithm = HashAlgorithm::SHA256);
        static NfHash fromMetadata(const std::vector<uint8_t>& metadata,
                                   HashAlgorithm algorithm = HashAlgorithm::SHA256);

        bool operator==(const NfHash& other) const;
        bool operator!=(const NfHash& other) const;

        HashType type() const;
        HashAlgorithm algorithm() const;

        bool isValid() const;

private:
        HashType m_type;
        HashAlgorithm m_algorithm;
        std::vector<uint8_t> m_value;
};

} // namespace NfCore

#endif // NF_PHOTO_HASH_H
