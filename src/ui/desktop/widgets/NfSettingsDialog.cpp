/**
 * File name: NfSettingsDialog.cpp
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

#include "NfSettingsDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>

namespace NfDesktop {

NfSettingsDialog::NfSettingsDialog(QWidget* parent)
        : QDialog(parent)
{
        setupUI();
        setupConnections();
        setWindowTitle("Nefluxon Settings");
        resize(600, 500);
}

void NfSettingsDialog::setupUI()
{
        m_tabs = new QTabWidget(this);

        // --- Thumbnail Cache Tab ---
        m_tabThumbnailCache = new QWidget;
        QFormLayout* form = new QFormLayout(m_tabThumbnailCache);

        m_cbDiskCacheSize = new QComboBox;
        m_cbDiskCacheSize->addItems({ "1 GB", "5 GB", "10 GB", "20 GB" });

        m_sliderRamCache = new QSlider(Qt::Horizontal);
        m_sliderRamCache->setRange(100, 5000);
        m_sliderRamCache->setValue(3000);
        m_labelRamCache = new QLabel("3000 Thumbnails");

        m_cbPreloadPages = new QComboBox;
        m_cbPreloadPages->addItems({ "1", "2", "3", "5" });

        m_cbThumbnailQuality = new QComboBox;
        m_cbThumbnailQuality->addItems({ "Low", "Medium", "High" });

        m_chkUseEmbeddedRaw = new QCheckBox("Use Embedded RAW Thumbnails");
        m_chkLazyLoading = new QCheckBox("Lazy Loading Mode");
        m_chkAutoDetect = new QCheckBox("Auto-Detect New Images");
        m_chkAutoCleanup = new QCheckBox("Automatic Disk Cleanup");

        m_editDiskCacheLocation = new QLineEdit;
        m_btnBrowse = new QPushButton("Browse...");
        m_btnClearDiskCache = new QPushButton("Clear Disk Cache");
        m_btnClearRamCache = new QPushButton("Clear RAM Cache");

        QHBoxLayout* diskLayout = new QHBoxLayout;
        diskLayout->addWidget(m_editDiskCacheLocation);
        diskLayout->addWidget(m_btnBrowse);

        QHBoxLayout* clearButtonsLayout = new QHBoxLayout;
        clearButtonsLayout->addWidget(m_btnClearDiskCache);
        clearButtonsLayout->addWidget(m_btnClearRamCache);

        form->addRow("Disk Cache Size:", m_cbDiskCacheSize);
        form->addRow("RAM Cache Limit:", m_sliderRamCache);
        form->addRow("", m_labelRamCache);
        form->addRow("Preload Pages:", m_cbPreloadPages);
        form->addRow("Thumbnail Quality:", m_cbThumbnailQuality);
        form->addRow("", m_chkUseEmbeddedRaw);
        form->addRow("", m_chkLazyLoading);
        form->addRow("", m_chkAutoDetect);
        form->addRow("", m_chkAutoCleanup);
        form->addRow("Disk Cache Location:", diskLayout);
        form->addRow("", clearButtonsLayout);

        m_tabs->addTab(m_tabThumbnailCache, "Thumbnail Cache");
        m_tabs->addTab(new QWidget, "Library");
        m_tabs->addTab(new QWidget, "UI");
        m_tabs->addTab(new QWidget, "Performance");

        // Bottom buttons
        m_btnOk = new QPushButton("OK");
        m_btnCancel = new QPushButton("Cancel");
        QHBoxLayout* buttonsLayout = new QHBoxLayout;
        buttonsLayout->addStretch();
        buttonsLayout->addWidget(m_btnOk);
        buttonsLayout->addWidget(m_btnCancel);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(m_tabs);
        mainLayout->addLayout(buttonsLayout);

        setLayout(mainLayout);
}

void NfSettingsDialog::setupConnections()
{
        connect(m_sliderRamCache, &QSlider::valueChanged, [this](int value){
                m_labelRamCache->setText(QString("%1 Thumbnails").arg(value));
        });

        connect(m_btnBrowse, &QPushButton::clicked, [this](){
                QString dir = QFileDialog::getExistingDirectory(this, "Select Disk Cache Folder");
                if (!dir.isEmpty())
                        m_editDiskCacheLocation->setText(dir);
        });

        connect(m_btnOk, &QPushButton::clicked, this, &QDialog::accept);
        connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

} // namespace NfDesktop
