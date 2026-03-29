/**
 * File name: NfPathLoader.h
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

#ifndef NF_PATH_LOADER_H
#define NF_PATH_LOADER_H

#incude "Neofluxon.h"

class NfPhotoPathLoader {
public:
        using PhotosLoadedCallback = std::function<void(std::vector<Photos>)>;

        PathLoader();
        ~PathLoader();

        void setPhotosLoadedCallback(PhotosLoadedCallback callback);
        void setPath(const std::string& path, bool recursive = true);
        std::filesystem::path getPath(const std::string& path) const;

srotected:
        void startScan();
        void scanDirectory(const std::string& path, bool recursive, std::stop_token stopToken);

private:
        mutable std::mutex m_mutex;
        std::vector<std::filesystem::path> m_files;
        std::vector<NfPhoto> m_photos;
        std::string m_path;
        bool m_recursive = true;
        PhotosLoadedCallback m_photosLoadedCallback;
        std::jthread m_scanThread;
        std::atomic<bool> m_stopFlag{false};
};

#endif // NF_PATH_LOADER_H
