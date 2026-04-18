/**
 * File name: NfFolderModeState.h
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

#include <QObject>

#include <filesystem>

namespace NfUi {

class NfUiFolderModeState : public QObject
{
        Q_OBJECT

public:
        enum class ViewMode {
                Grid,
                Preview
        };

        explicit NfUiFolderModeState(QObject* parent = nullptr);
        ViewMode viewMode() const;
        const std::filesystem::path& path() const;

signals:
        void viewModeChanged(ViewMode mode);
        void pathChanged(const std::filesystem::path& path);

public slots:
        void setViewMode(ViewMode mode);
        void setPath(const std::filesystem::path& path);

private:
        ViewMode m_viewMode;
        std::filesystem::path m_path;
};
} // namespace NfUi
