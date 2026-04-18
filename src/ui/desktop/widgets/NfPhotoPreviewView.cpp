/**
 * File name: NfPhotoPreviewView.cpp
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

#include "NfPhotoPreviewView.h"
#include "NfBrowserModel.h"

#include <QLabel>
#include <QVBoxLayout>

namespace NfDesktop {

NfPhotoPreviewView::NfPhotoPreviewView(NfBrowserModel *model, QWidget* parent)
        : QWidget(parent)
        , m_model{model}
        , m_photoPreview{new QLabel(this)}
{
        m_photoPreview->setAlignment(Qt::AlignCenter);
        m_photoPreview->setScaledContents(true);

        auto layout = new QVBoxLayout(this);
        layout->addWidget(m_photoPreview);

        connectModel();
        updateView();
}

void NfPhotoPreviewView::setModel(NfBrowserModel* model)
{
        if (m_model == model)
                return;

        if (m_model)
                QObject::disconnect(m_model, nullptr, this, nullptr);

        m_model = model;

        connectModel();
        updateView();
}

NfBrowserModel* NfPhotoPreviewView::model() const
{
        return m_model;
}

void NfPhotoPreviewView::connectModel()
{
        if (!m_model)
                return;

        QObject::connect(m_model,
                         &NfBrowserModel::modelUpdated,
                         this,
                         &NfPhotoPreviewView::updateView);
        QObject::connect(m_model,
                         &NfBrowserModel::currentPhotoChanged,
                         this,
                         &NfPhotoPreviewView::updateView);
        QObject::connect(m_model,
                         &NfBrowserModel::currentPreviewReady,
                         this,
                         &NfPhotoPreviewView::updateView);
}

void NfPhotoPreviewView::updateView()
{
        if (!m_model) {
                m_photoPreview->clear();
                return;
        }

        m_photoPreview->setPixmap(m_model->currentPreview());
}

} // namespace NfDesktop
