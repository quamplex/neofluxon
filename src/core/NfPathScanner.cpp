/**
 * File name: NfPathScanner.cpp
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

#include "NfPathScanner.h"

NfPathScanner::NfPathScanner()
        : m_recursive{false}
        , m_startScan{false}
{
        m_scanThread = std::jthread(&PathLoader::scanDirectory, this);
}

void NfPathScanner::setPath(const std::string& path, bool recursive)
{
        {
                std::lock_guard lock(m_mutex);
                m_directory = path;
                m_recursive = recursive;
                m_startScan.store(true, std::memory_order_relaxed);
        }

        m_conditionVariable.notify_one();
}

void NfPathScanner::loadPhotosThread(std::stop_token stopToken)
{
        namespace fs = std::filesystem;

        while (!stopToken.stop_requested()) {
                std::string directory;
                bool recursive;

                {
                        std::unique_lock lock(m_mutex);

                        m_conditionVariable.wait(lock, [&]() {
                                return stopToken.stop_requested() ||
                                        m_startScan.load(std::memory_order_relaxed);
                        });

                        if (stopToken.stop_requested()) {
                                m_loadedPhotos.clear();
                                return;
                        }

                        m_loadedPhotos.clear();

                        directory = m_directory;
                        recursive = m_recursive;
                        m_startScan.store(false, std::memory_order_relaxed);
                }

                try {
                        if (recursive) {
                                fs::recursive_directory_iterator it(directory), end;

                                for (; it != end; ++it) {
                                        if (stopToken.stop_requested())
                                                return;

                                        if (m_startScan.load(std::memory_order_relaxed))
                                                break;

                                        processPathEntry(it->path());
                                }
                        } else {
                                fs::directory_iterator it(directory), end;

                                for (; it != end; ++it) {
                                        if (stopToken.stop_requested())
                                                return;

                                        if (m_startScan.load(std::memory_order_relaxed))
                                                break;

                                        processPathEntry(it->path());
                                }
                        }
                }
                catch (const std::filesystem::filesystem_error&) {
                }
        }
}

void NfPathScanner::processPathEntry(const std::filesystem::path& path)
{
        namespace fs = std::filesystem;

        try {
                if (!fs::is_regular_file(path))
                        return;

                std::string ext = path.extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(),
                               [](unsigned char c) { return std::tolower(c); });

                if (!m_photoExtentions.contains(ext))
                        return;

                NfPhoto photo(path);

                {
                        std::lock_guard lock(m_mutex);
                        m_loadedPhotos.push_back(std::move(photo));
                }
        }
        catch (const fs::filesystem_error&) {
        }
}

std::vector<NfPhoto> NfPathScanner::takePhotos()
{
        std::lock_guard lock(m_mutex);
        return std::move(m_loadedPhotos);
}
