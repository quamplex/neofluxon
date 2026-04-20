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
#include "NfUiState.h"
#include "NfUiFolderModeState.h"
#include "NfFolderModel.h"
#include "NfBrowserModel.h"
#include "NfBrowserView.h"
#include "NfPhotoPreviewView.h"

#include <QVBoxLayout>

using namespace NfUi;

namespace NfDesktop {

NfFolderView::NfFolderView(const NfFolderContext &ctx,
                           NfFolderModel *model,
                           QWidget* parent)
        : QWidget(parent)
        , m_context{ctx}
        , m_state{ctx.uiState()->folderModeState()}
        , m_model{model}
        , m_mainLayout{nullptr}
        , m_browserView{nullptr}
        , m_photoPreviewView{nullptr}
{
        m_mainLayout = new QVBoxLayout(this);
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->setSpacing(0);

        m_browserView = new NfBrowserView(this);
        m_browserView->setModel(m_model->browser());
        m_mainLayout->addWidget(m_browserView);

        QObject::connect(m_state,
                         &NfUiFolderModeState::viewModeChanged,
                         this,
                         &NfFolderView::updateView);
        QObject::connect(m_browserView, &QListView::doubleClicked,
                         [this](const QModelIndex &index) {
                                 m_state->setViewMode(NfUiFolderModeState::ViewMode::Preview);
                         });

        updateView();
}

void NfFolderView::showGridView()
{
        m_browserView->setLayoutMode(NfBrowserView::LayoutMode::GridView);

        if (m_photoPreviewView) {
                m_mainLayout->removeWidget(m_photoPreviewView);
                m_photoPreviewView->deleteLater();
                m_photoPreviewView = nullptr;
        }

        m_mainLayout->setStretch(0, 1);
}

void NfFolderView::showPreviewView()
{
        m_browserView->setLayoutMode(NfBrowserView::LayoutMode::FilmstripView);

        if (!m_photoPreviewView) {
                m_photoPreviewView = new NfPhotoPreviewView(m_model->browser(), this);
                QObject::connect(m_browserView, &QListView::clicked,
                                 m_photoPreviewView,
                                 &NfPhotoPreviewView::setPhotoIndex);

                // Insert at the top
                m_mainLayout->insertWidget(0, m_photoPreviewView);

                // Index 0 (Preview) gets more space, Index 1 (Filmstrip) gets less
                m_mainLayout->setStretch(0, 7);
                m_mainLayout->setStretch(1, 3);

                m_photoPreviewView->setPhotoIndex(m_browserView->currentIndex());
                m_browserView->scrollTo(m_browserView->currentIndex(), QAbstractItemView::PositionAtCenter);
        }
}

void NfFolderView::updateView()
{
        switch(m_state->viewMode()) {
        case NfUiFolderModeState::ViewMode::Preview:
                showPreviewView();
                break;
        case NfUiFolderModeState::ViewMode::Grid:
        default:
                showGridView();
                break;
        }
}

} // namespace NfDesktop
