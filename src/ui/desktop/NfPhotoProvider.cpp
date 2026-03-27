/**
 * File name: NfPhotoProvider.cpp
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

#include "NfPhotoProvider.h"

NfPhotoProvider::NfPhotoProvider(NfPhotoLoader& m_photoLoader,
                                 NfGuiCache &cache,
                                 QObject* parent)
        : QtObject(parent)
        , m_cache{cache}
        , m_photoLoader{cache}
{
        // Callback for new photos
        m_photoLoader->setPhotosLoadedCallback([this](std::vector<NfPhoto> newPhotos) {
                QMetaObject::invokeMethod(this, [this, newPhotos = std::move(newPhotos)]() mutable
                {
                        emit photosLoaded(std::move(newPhotos));
                },
                Qt::QueuedConnection);
        });

        // Thumbnails loaded callback
        m_photoProvider->setThumbnailsLoadedCallback([this](std::vector<NfGuiThumbail> thumbnails) {
                QMetaObject::invokeMethod(this, [this, thumbnails = std::move(thumbnails)]() mutable {
                        std::vector<NfPhotoId> ids;
                        ids.reserve(thumbnails.size());

                        for (auto& thumbnail : thumbnails) {
                                m_cache->add(thumbnail.id(), thumbnail.releaseImage());
                                ids.push_back(thumbnail.id());
                        }

                        emit thumbnailsLoaded(ids);
                }, Qt::QueuedConnection);
        });
}

void NfPhotoProvider::setPath(const std::filesystem::path& path)
{
        m_path = path;
        m_photoLoader.setPath(path);
}

const std::filesystem::path& NfPhotoProvider::getPath() const
{
        return m_path;
}

QPixmap& NfBrowserModel::getThumbnail(const std::unique_ptr<NfPhoto> &photo) const
{
        auto const *cacheImage = m_cache.getThumbail(photo->id());
        if (cacheImage) {
                auto const *thumbnail = dynamic_cast<const NfQtPixmap*>(cacheImage);
                if (thumbnail)
                        return thumbnail->pixmap();
        }

        m_photoLoader.requestThumbnail(photo, std::make_unique<NfQtPixmap>());

        return m_thumbnailPlaceholder;
}







