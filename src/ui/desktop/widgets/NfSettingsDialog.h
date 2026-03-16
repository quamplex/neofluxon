/**
 * File name: NfSettingsDialog.h
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

#ifndef NF_SETTINGS_DIALOG_H
#define NF_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>
#include <QWidget>

namespace Desktop {

class NfSettingsDialog : public QDialog
{
        Q_OBJECT

 public:
        explicit NfSettingsDialog(QWidget* parent = nullptr);

        // getters for settings
        int diskCacheSize() const;
        int ramCacheLimit() const;
        int preloadPages() const;
        QString thumbnailQuality() const;
        int asyncThreads() const;
        bool useEmbeddedRaw() const;
        bool lazyLoading() const;
        bool autoDetectImages() const;
        bool autoDiskCleanup() const;
        QString diskCacheLocation() const;

 private:
        void setupUI();
        void setupConnections();

        QTabWidget* m_tabs;

        // Thumbnail Cache Tab
        QWidget* m_tabThumbnailCache;
        QComboBox* m_cbDiskCacheSize;
        QSlider* m_sliderRamCache;
        QLabel* m_labelRamCache;
        QComboBox* m_cbPreloadPages;
        QComboBox* m_cbThumbnailQuality;
        QCheckBox* m_chkUseEmbeddedRaw;
        QCheckBox* m_chkLazyLoading;
        QCheckBox* m_chkAutoDetect;
        QCheckBox* m_chkAutoCleanup;
        QLineEdit* m_editDiskCacheLocation;
        QPushButton* m_btnBrowse;
        QPushButton* m_btnClearDiskCache;
        QPushButton* m_btnClearRamCache;

        // Buttons
        QPushButton* m_btnOk;
        QPushButton* m_btnCancel;
};

} // namespace Desktop

#endif // NF_SETTINGS_DIALOG_H
