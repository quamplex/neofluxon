/**
 * File name: MainWindow.h
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

#include "LqfMainWindow.h"
#include "NfPanel.h"

#include <QFrame>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Desktop {

// ---------------------------------------------------------------
// Helpers (unchanged except for a slightly stronger shadow)
// ---------------------------------------------------------------
static QFrame* buildTopbar(int height)
{
        QFrame *panel = new QFrame;
        panel->setFixedHeight(height);
        panel->setObjectName("NfTopBar");
        panel->setAttribute(Qt::WA_StyledBackground, true);
        return panel;
}

LqfMainWindow::LqfMainWindow()
        : QMainWindow()
{
        resize(1400, 800);
        setGeometry(200, 200, 1400, 800);

        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);

        auto container = new QWidget(this);
        container->setObjectName("container");

        // -----------------------------------------------------------
        // Vertical layout – top bar + content area
        // -----------------------------------------------------------
        auto *vLayout = new QVBoxLayout(container);
        vLayout->setContentsMargins(0,0,0,0);
        vLayout->setSpacing(0);   // no extra vertical space; shadow draws into the gap

        // -----------------------------------------------------------
        //  Add the top bar (shadow goes downwards)
        // -----------------------------------------------------------
        auto topBar = buildTopbar(48);
        vLayout->addWidget(topBar);

        // -----------------------------------------------------------
        //  Horizontal layout for left / centre / right
        // -----------------------------------------------------------
        auto *hLayout = new QHBoxLayout;
        hLayout->setContentsMargins(0,0,0,0);
        hLayout->setSpacing(0);

        // Left panel (shadow → centre)
        auto leftPanel = new NfPanel(nullptr, NfPanel::PanelPosition::AlignLeft);
        hLayout->addWidget(leftPanel);

        // Central widget – any background you like.
        auto *centralWidget = new QWidget;
        centralWidget->setObjectName("NfCentralWidget");

        hLayout->addWidget(centralWidget);   // stretch factor 1 → fill width

        // Right panel (shadow ← centre)
        auto rightPanel = new NfPanel(nullptr, NfPanel::PanelPosition::AlignRight);
        hLayout->addWidget(rightPanel);

        // Wrap the horizontal layout in a dummy widget so it can be added
        // to the vertical layout as a single item.
        QWidget *contentWrapper = new QWidget;
        contentWrapper->setAttribute(Qt::WA_TranslucentBackground);
        contentWrapper->setLayout(hLayout);
        vLayout->addWidget(contentWrapper);   // stretch factor 1 → fill remaining height

        leftPanel->raise();
        rightPanel->raise();
        topBar->raise();

        setCentralWidget(container);
        //showMaximized();
}

LqfMainWindow::~LqfMainWindow()
{
}

} // namespace Desktop
