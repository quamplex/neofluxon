/**
 * File name: MainWindow.h
 * Project: Liquid Flow (A photo management software)
 *
 * Copyright (C) 2022 Iurie Nistor
 *
 * This file is part of Liquid Flow.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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
#include "LqfPanel.h"

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
        panel->setObjectName("topBar");

        // Make sure the style‑sheet background is really painted
        panel->setAttribute(Qt::WA_StyledBackground, true);

        // Background colour – change to match your theme

        /*panel->setStyleSheet(
                             "QFrame {"
                             "  background-color: #201f27;"
                             "  border: 3px solid #23222b;"
                             "  border-top: 5px solid #23222b;"
                             "  border-radius: 10px;"
                             "}"
                             );*/

        panel->setStyleSheet(
                             ""
                             "  background-color: #201f27;"
                             "  border-left: 3px solid #23222b;"
                             "  border-right: 3px solid #23222b;"
                             "  border-bottom: 3px solid #23222b;"
                             "  border-top: 3px solid #23222b;"

                             "  border-top-left-radius: 8px;"
                             "  border-top-right-radius: 8px;"
                             "  border-bottom-left-radius: 0px;"
                             "  border-bottom-right-radius: 0px;"
                             );

        return panel;
}

LqfMainWindow::LqfMainWindow()
        : QMainWindow()
{
        resize(1400, 800);
        setGeometry(200, 200, 1400, 800);

        // Give the whole window a dark colour – this will be the fallback
        // if anything else fails to paint.
        //setStyleSheet("background-color:#18171e;");

        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);

        auto container = new QWidget(this);
        container->setObjectName("container");

        setStyleSheet(
                      "#container {"
                      "  background-color: #18171e;"
                      "  border: 3px;"
                      "  border-top: 5px;"
                      "  border-radius: 10px;"
                      "}"
                      );

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

        vLayout->addSpacing(2);

        // -----------------------------------------------------------
        //  Horizontal layout for left / centre / right
        // -----------------------------------------------------------
        auto *hLayout = new QHBoxLayout;
        hLayout->setContentsMargins(0,0,0,0);
        hLayout->setSpacing(0);

        // Left panel (shadow → centre)
        auto leftPanel = new LqfPanel(nullptr, LqfPanel::PanelPosition::Left);
        leftPanel->setFixedWidth(250);
        hLayout->addWidget(leftPanel);
        leftPanel->show();

        // Central widget – any background you like.
        auto *centralWidget = new QWidget;
        centralWidget->setStyleSheet(
                                     "background-color:#18171e;"   // same colour as window, or change it
                                     "border: 0px;"
                                     "border-bottom: 3px solid #23222b;"
                                     );

        hLayout->addWidget(centralWidget);   // stretch factor 1 → fill width

        // Right panel (shadow ← centre)
        auto rightPanel = new LqfPanel(nullptr, LqfPanel::PanelPosition::Right);
        rightPanel->setFixedWidth(250);
        hLayout->addWidget(rightPanel);

        // Wrap the horizontal layout in a dummy widget so it can be added
        // to the vertical layout as a single item.
        QWidget *contentWrapper = new QWidget;
        contentWrapper->setLayout(hLayout);
        vLayout->addWidget(contentWrapper);   // stretch factor 1 → fill remaining height

        leftPanel->raise();
        rightPanel->raise();
        topBar->raise();

        setCentralWidget(container);
        //        showMaximized();
}

LqfMainWindow::~LqfMainWindow()
{
}

} // namespace Desktop
