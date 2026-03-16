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

namespace Desktop {

NfBrowserModel::NfBrowserModel(QObject* parent)
        : QAbstractListModel(parent)
{
}

int NfBrowserModel::rowCount(const QModelIndex& parent) const
{
        if (parent.isValid())
                return 0;
        return m_photos.size();
}

QVariant NfBrowserModel::data(const QModelIndex& index, int role) const
{
        if (!index.isValid() || index.row() >= m_photos.size())
                return QVariant();

        switch (role) {
                case Qt::DecorationRole:
                        return m_photos[index.row()];   // thumbnail pixmap
                case Qt::DisplayRole:
                        return QString("Photo %1").arg(index.row() + 1); // optional text
                default:
                        return QVariant();
        }
}

void NfBrowserModel::setPhotoCount(int count)
{
        beginResetModel();
        m_photos.clear();

        QPixmap pixmap(":/thumb_w160.jpg");

        for (int i = 0; i < count; ++i) {
                m_photos.append(pixmap);
        }

        endResetModel();
}

} // namespace Desktop
