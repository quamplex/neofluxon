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
#include "NfBrowser.h"

using namespace NfCore;

namespace Desktop {

NfBrowserModel::NfBrowserModel(NfBrowser* browser, QObject* parent)
        : QAbstractListModel(parent)
        , m_browserCore{browser}
{
        m_browserCore->setItemNumberChangedCallback([this]() {
                QMetaObject::invokeMethod(this, [this]() {
                        // Notify the view that the rows may have changed.
                        // The view will query rowCount() again.
                        beginResetModel();
                        endResetModel();
                }, Qt::QueuedConnection);
        });

        m_browserCore->setThumbnailReadyCb([this](size_t index) {
                // Queue the thumbnail update on the main GUI thread
                // Only do this if the thumbnail is currently visible.
                QMetaObject::invokeMethod(this, [this, index]() {
                        if (!isIndexVisible(index)) {
                                // User scrolled away, drop this thumbnail.
                                return;
                        }

                        QModelIndex modelIndex = this->index(index);
                        emit dataChanged(modelIndex, modelIndex, {Qt::DecorationRole});
                }, Qt::QueuedConnection);
        });
}

int NfBrowserModel::rowCount(const QModelIndex& parent) const
{
        if (parent.isValid())
                return 0;
        return m_browserCore->numberOfPhotos();
}

QVariant NfBrowserModel::data(const QModelIndex& index, int role) const
{
        if (!index.isValid() || index.row() >= m_photos.size())
                return QVariant();

        switch (role) {
        case Qt::DecorationRole:
        {
                if (auto* thumbnail = dynamic_cast<const NfQtPixmap*>(m_browserCore->getThumbnail(row)))
                        return thumbnail->pixmap();
                return QPixmap(150, 150);
        }
        case Qt::DisplayRole:
                // Optional text label
                return QString("Photo %1").arg(index.row() + 1);
        default:
                return QVariant();
        }
}

} // namespace Desktop
