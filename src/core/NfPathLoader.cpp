/**
 * File name: NfPathLoader.cpp
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

#include "PathLoader.h"

using namespace std::chrono_literals;

PathLoader::PathLoader()
        : m_recursive {false}
{
}

PathLoader::~PathLoader()
{
    {
        std::lock_guard lock(m_mutex);
        m_photosLoadedCallback = nullptr;
    }

    if (m_scanThread.joinable()) {
        m_stopFlag = true;
        m_scanThread.join();
    }
}

void PathLoader::setPath(const std::string& path, bool recursive)
{
        std::lock_guard lock(m_mutex);
        m_directory = path;
        m_recursive = recursive;
}

void PathLoader::startScan() {
    std::string directoryCopy;
    bool recursiveCopy;

    {
        std::lock_guard lock(m_mutex);
        directoryCopy = m_directory;
        recursiveCopy = m_recursive;
    }

    m_scanThread = std::jthread([this, directoryCopy, recursiveCopy](std::stop_token stopToken) {
        scanDirectory(directoryCopy, recursiveCopy, stopToken);
    });
}

void PathLoader::scanDirectory(const std::string& path,
                               bool recursive,
                               std::stop_token stopToken)
{
        namespace fs = std::filesystem;

        auto lastNotifyTime = std::chrono::steady_clock::now();
        auto shouldNotify = [&]() {
                auto now = std::chrono::steady_clock::now();
                return (now - lastNotifyTime > 100ms);
        };

        std::vector<NfPhoto> loadedPhotos;
        std::function<void(const fs::path&)> scanRecursive;
        scanRecursive = [&](const fs::path& p) {
                for (auto& entry : fs::directory_iterator(p)) {
                        if (stopToken.stop_requested())
                                return;

                        if (entry.is_directory() && recursive)
                                scanRecursive(entry.path());

                        if (entry.is_regular_file()) {
                                loadedPhotos.push_back(NfPhoto(entry.path()));
                                if (shouldNotify()) {
                                        lastNotifyTime = std::chrono::steady_clock::now();
                                        std::lock_guard lock(m_mutex);
                                        if (m_photosLoadedCallback)
                                                m_photosLoadedCallback(std::move(loadedPhotos));
                                }
                        }
                }
        };

        if (!path.empty())
                scanRecursive(path);

        // Final notification after scan completes
        if (!m_photos.empty()) {
                std::lock_guard lock(m_mutex);
                if (m_photosLoadedCallback)
                        m_photosLoadedCallback(std::move(loadedPhotos));
        }
}
