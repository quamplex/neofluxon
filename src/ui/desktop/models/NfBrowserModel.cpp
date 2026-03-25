/**
 * File name: NfBrowserModel.cpp
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

#include "NfBrowserModel.h"
#include "NfCacheQtPixmap.h"
#include "NfPhotoProvider.h"
#include "NfGuiCache.h"
#include "NfPhotoInfo.h"
#include "NfThumbnail.h"

using namespace NfCore;

namespace Desktop {

NfBrowserModel::NfBrowserModel(NfPhotoProvider* photoProvider,
                               NfGuiCache *cache,
                               QObject* parent)
        : QAbstractListModel(parent)
        , m_photoProvider{photoProvider}
        , m_cache{cache}
        , m_thumbnailPlaceholder{QPixmap(150, 150)}
{
        // Callback for new photos
        m_photoProvider->setNewPhotosCallback([this](std::vector<std::unique_ptr<NfPhotoInfo>> newPhotos) {
                QMetaObject::invokeMethod(this, [this, newPhotos = std::move(newPhotos)]() mutable {

                        const int firstRow = static_cast<int>(m_photos.size());
                        const int count    = static_cast<int>(newPhotos.size());
                        const int lastRow  = firstRow + count - 1;

                        beginInsertRows(QModelIndex(), firstRow, lastRow);

                        m_photos.reserve(m_photos.size() + count);
                        std::ranges::for_each(newPhotos, [this](auto& ptr) {
                                m_photos.push_back(std::move(ptr));
                        });

                        // Update map: PhotoId -> QPersistentModelIndex
                        for (int i = 0; i < count; ++i) {
                                const auto& photoInfo = m_photos[firstRow + i];
                                m_itemsMap[photoInfo->id()] = QPersistentModelIndex(index(firstRow + i));
                        }

                        endInsertRows();
                }, Qt::QueuedConnection);
        });

        // Thumbnails ready callback
        m_photoProvider->setThumbnailsReadyCb([this](std::vector<NfThumbnail> thumbnails) {
                QMetaObject::invokeMethod(this, [this, thumbnails = std::move(thumbnails)]() mutable {
                        std::ranges::for_each(thumbnails, [this](auto& thumb) {

                                // Add thubnail image to cache.
                                m_cache->addThumbail(thumb.id(), std::move(thumbnails.releaseImage()));

                                if (auto it = m_itemsMap.find(thumb.id()); it != m_itemsMap.end()) {
                                        const auto& idx = it->second;
                                        if (idx.isValid() && isIndexVisible(idx))
                                                emit dataChanged(idx, idx, {Qt::DecorationRole});
                                }
                        });
                }, Qt::QueuedConnection);
        });
 }

void NfBrowserModel::~NfBrowserModel()
{
        m_photoProvider->setNewPhotosCallback({});
        m_photoProvider->setThumbnailsReadyCb({});
}

void NfBrowserModel::setPath(const std::filesystem::path &path)
{
        beginResetModel();
        m_photos.clear();
        m_itemsMap.clear();
        endResetModel();

        m_photoProvider->loadPath(m_path);
}

const std::filesystem::path& int NfBrowserModel::getPath() const
{
        return m_path;
}

int NfBrowserModel::rowCount(const QModelIndex& parent) const
{
        return parent.isValid() ? 0 : static_cast<int>(m_photos.size());
}

QVariant NfBrowserModel::data(const QModelIndex& index, int role) const
{
        if (!index.isValid() || index.row() >= m_photos.size())
                return QVariant();

        switch (role) {
        case Qt::DecorationRole:
                return getThumbnail(index);
        case Qt::DisplayRole:
                return QString("Photo %1").arg(index.row() + 1);
        default:
                return QVariant();
        }
}

QVariant NfBrowserModel::getThumbnail(const QModelIndex &index)
{
        const auto& photoInfo = m_photos[index.row()];
        auto const *cahceObject = m_cache->getThumbnail(photoInfo->id());
        if (cahceObject) {
                auto const *thumbnail = dynamic_cast<const NfCacheQtPixmap*>(cahceObject);
                if (thubnail)
                        return thumbnail->pixmap();
        }

        m_photoProvider->requestThumbnail(photoInfo, std::make_unique<NfQtPixmap*>());

        return m_thumbnailPlaceholder;
}

} // namespace Desktop
