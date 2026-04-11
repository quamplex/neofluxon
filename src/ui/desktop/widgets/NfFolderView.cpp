/**
 * File name: NfFolderView.cpp
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

#include "NfFolderView.h"
#include "NfFilderModeState.h"

namespace NfDesktop {

NfFolderView::NfFolderView(NfFilderModeState *state,
                           NfBrowserModel *model,
                           QWidget* parent)
        : QWidget(parent)
        , m_state{state}
        , m_model{model}
        , m_mainLayout{nullptr}
        , m_browserView{nullptr}
        , m_photoPreviewView{nullptr}
{
        m_mainLayout = new QVBoxLayout(this);
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->setSpacing(0);

        m_browserView = new NfBrowserView(this, m_model);

        m_mainLayout->addWidget(m_browserView);

        QObject::connect(m_state,
                         &NfFilderModeState::viewModeChanged,
                         this,
                         &NfFolderView::updateView);

        updateView();
}

void NfFolderView::showGridView()
{
        m_browserView->setLayoutMode(NfBrowserView::GridView);

        if (m_photoPreviewView) {
                m_mainLayout->removeWidget(m_photoPreviewView);
                m_photoPreviewView->deleteLater();
                m_photoPreviewView = nullptr;
        }

        m_mainLayout->setStretch(0, 1);
}

void NfFolderView::showPreviewView()
{
        m_browserView->setLayoutMode(NfBrowserView::FilmstripView);

        if (!m_photoPreviewView) {
                m_photoPreviewView = new NfPhotoPreviewView(this, m_model);

                // Insert at the top
                m_mainLayout->insertWidget(0, m_photoPreviewView);

                // Index 0 (Preview) gets more space, Index 1 (Filmstrip) gets less
                m_mainLayout->setStretch(0, 7);
                m_mainLayout->setStretch(1, 3);
        }
}

void NfFolderView::updateView()
{
        switch(m_state->viewMode()) {
        case NfFilderModeState::ViewMode::Preview:
                showPreviewView();
                break;
        case NfFilderModeState::View::Grid:
        default:
                showGridView();
                break;
        }
}

} // namespace NfDesktop

#endif // NF_FOLDER_VIEW_H
