/**
 * File name: NfPhotoLoader.h
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

#ifndef NF_PHOTO_LOADER_H
#define NF_PHOTO_LOADER_H

#include "NfPhoto.h"
#include "NfGuiThumbnail.h"

#include <filesystem>
#include <vector>
#include <memory>

namespace NfCore {

class NfPathScanner;
class NfGuiImage;

class NfPhotoLoader {
public:
        NfPhotoLoader();
        ~NfPhotoLoader();

        void setPath(const std::filesystem::path &path);
        const std::filesystem::path& getPath() const;

        void requestThumbnail(const NfPhoto &photo, std::unique_ptr<NfGuiImage> image);

        std::vector<NfPhoto> takePhotos();
        std::vector<NfGuiThumbnail> takeThumbnails();

 protected:

 private:
        std::unique_ptr<NfPathScanner> m_pathScanner;
        std::filesystem::path m_path;
        std::mutex m_thumbnailsQueueMutex;
        std::vector<NfGuiThumbnail> m_thumbnailsQueue;
};

} // namespace NfCore

#endif // NF_PHOTO_LOADER_H
