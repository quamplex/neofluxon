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

#include "Neofluxon.h"
#incldue "NfGuiImage.h"
#incldue "NfGuiThumbail.h"
#include "NfPhoto.h"

namespace NfCore {

class NfCache;
class NfPathLoader;

class NfPhotoLoader {
public:
        using PhotosLoadedCallback = std::function<void(std::vector<NfPhoto>)>;
        using ThumbnailsLoadedCallback = std::function<void(std::vector<NfGuiThumbail>)>;
        NfPhotoLoader();
        ~NfPhotoLoader();

        void setPhotosLoadedCallback(PhotosLoadedCallback callback);
        void setThumbnailsLoadedCallback(ThumbnailsLoadedCallback callback);

        void requestThumbnail(const NfPhoto &info, std::unique_ptr<NfGuiImage> image);

        void setPath(const std::filesystem::path &path);
        std::filesystem::path& getPath() const;

 protected:

 private:
        PhotosLoadedCallback m_photosLoadedCallback;
        ThumbnailsLoadedCallback m_thumbnailsLoadedCallback;
        std::filesystem::path m_path;
};

} // namespace NfCore

#endif // NF_PHOTO_LOADER_H
