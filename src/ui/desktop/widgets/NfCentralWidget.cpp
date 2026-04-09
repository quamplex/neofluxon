/**
 * File name: NfCentralWidget.h
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

#include "NfCentralWidget.h"
#include "NfThumbnailsModeView.h"
#include "NfPreviewModeView.h"

#include <QVBoxLayout>

namespace NfDesktop {

NfCentralWidget::NfCentralWidget(QWidget* parent)
        : QWidget(parent)
        , m_centralStack{new QStackedWidget(this)}
        , m_thumbnailsModeView{nullptr},
        , m_previewModeView{nullptr}
        , m_editModeView{nullptr}
        , m_browserModel{new NfBrowserModel(m_photoProvider, this)}
{
        setObjectName("NfCentralWidget");

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_centralStack);

        setLayout(layout);
}

void NfCentralWidget::showThumbnailsMode()
{
        setCurrentView(new NfThumbnailModeView(this, m_browserModel));
}

void NfCentralWidget::showPreviewMode()
{
        setCurrentView(new NfPreviewModeView(this, m_browserModel));
}

void NfCentralWidget::setCurrentView(QtWidget* view)
{
        auto currentView = m_centralStack->currentWidget();
        if (currentView) {
                m_centralStack->removeWidget(currentView);
                delete currentView;
        }

        m_centralStack->setCurrentWidget(view);
}

} // namespace NfDesktop
