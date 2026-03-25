/**
 * File name: NfBrowserModel.h
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

#ifndef NF_BROWSER_MODEL_H
#define NF_BROWSER_MODEL_H

#include "NfPhotoId.h"

#include <QAbstractListModel>
#include <QPixmap>

namespace Desktop {

class NfPhotoProvider;
class NfGuiCache;
class NfPhotoInfo;

class NfBrowserModel : public QAbstractListModel
{
        Q_OBJECT

public:
        explicit NfBrowserModel(NfPhotoProvider* browserProxy,
                                NfGuiCache* cache,
                                QObject* parent = nullptr);
        ~NfBrowserModel();

        void setPath(const std::filesystem::path& path);
        const std::filesystem::path& getPath() const;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

protected:
        QVariant getThumbnail(const QModelIndex& index) const;

private:
        NfPhotoProvider* m_photoProvider;
        NfGuiCache* m_cache;
        std::vector<std::unique_ptr<NfPhotoInfo>> m_photos;
        std::unordered_map<NfPhotoId, QPersistentModelIndex> m_itemsMap;
        std::filesystem::path m_path;
        QPixmap m_thumbnailPlaceholder;
};

} // namespace Desktop

#endif // NF_BROWSER_MODEL_H
