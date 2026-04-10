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

namespace NfDesktop {

NfFolderView::NfFolderView(NfFilderModeState *state,
                           NfBrowserModel *model,
                           QWidget* parent = nullptr)
        : QWidget(parent)
        , m_state{state}
        , m_model{model}
{
        updateView();
}

void NfFolderView::showGridView()
{
        m_borwserView->setLayoutMode(NfBrowserView::GridView);
        if (m_photoPreviewView) {
                m_photoPreviewView->deleteLater();
                m_photoPreviewView = nullptr;
        }
}

void NfFolderView::showPreviewView()
{
        m_borwserView->setLayoutMode(NfBrowserView::FilmstripView);
        if (!m_photoPreviewView)
                m_photoPreviewView = new NfPhotoPreviewView(this, model);
}

NfFolderView::updateView()
{
        auto* state = m_ui->getFolderModeState();
        switch(m_state->viewMode()) {
        case NfFilderModeState::ViewMode::Preview:
                showPreviewView();
                break;
        case NfFilderModeState::View::Grid:
                showGridView();
                break;
        }
} // namespace NfDesktop

#endif // NF_FOLDER_VIEW_H
