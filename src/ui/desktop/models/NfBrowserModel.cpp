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
#include "NfPhotoProvider.h"
#include "NfPhoto.h"

using namespace NfCore;

namespace Desktop {

NfBrowserModel::NfBrowserModel(NfPhotoProvider &photoProvider, QObject* parent)
        : QAbstractListModel(parent)
        , m_photoProvider{photoProvider}
{
        QtObject::connect(m_thumbnailProvider,
                          &NfPhotoProvider::photosLoaded,
                          this,
                          &NfBrowserModel::onPhotosLoaded);
        QtObject::connect(m_thumbnailProvider,
                          &NfPhotoProvider::thumbnailsLoaded,
                          this,
                          &NfBrowserModel::onThumbnailsLoaded);
 }

void NfBrowserModel::~NfBrowserModel()
{
}

void NfBrowserModel::setPath(const std::filesystem::path &path)
{
        beginResetModel();
        m_photoIds.clear();
        m_itemsMap.clear();
        endResetModel();

        m_photoProvider->setPath(m_path);
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
                return m_photoProvider->getThumbnail(m_photos[index.row()]);
        case Qt::DisplayRole:
                return QString("Photo %1").arg(index.row() + 1);
        default:
                return QVariant();
        }
}

void NfBrowserModel::onPhotosLoaded(std::vector<NfPhoto> newPhotos)
{
        const int firstRow = static_cast<int>(m_photos.size());
        const int count    = static_cast<int>(newPhotos.size());
        const int lastRow  = firstRow + count - 1;

        beginInsertRows(QModelIndex(), firstRow, lastRow);

        m_photos.reserve(m_photos.size() + count);
        m_photos.insert(m_photos.end(),
                        std::make_move_iterator(newPhotos.begin()),
                        std::make_move_iterator(newPhotos.end()));

        for (int i = 0; i < count; ++i) {
                const auto& photo = m_photos[firstRow + i];
                m_itemsMap[photo.id()] = QPersistentModelIndex(index(firstRow + i));
        }

        endInsertRows();
}

void NfBrowserModel::onThumbnailsLoaded(const std::vector<NfPhotoId> ids)
{
        std::ranges::for_each(ids, [this](auto& id) {
                if (auto it = m_itemsMap.find(thumb.id()); it != m_itemsMap.end()) {
                        const auto& idx = it->second;
                        if (idx.isValid() && isIndexVisible(idx))
                                emit dataChanged(idx, idx, {Qt::DecorationRole});
                }
        });
}

} // namespace Desktop
