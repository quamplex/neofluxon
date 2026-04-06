/**
 * File name: NfMainMenu.cpp
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

#include "NfMainMenu.h"

#include <QVBoxLayout>
#include <QPushButton>

namespace NfDesktop {

NfMainMenu::NfMainMenu(QWidget* parent)
        : NfStyledWidget(parent)
        , shootsButton{nullptr}
        , foldersButton{nullptr}
        , libraryButton{nullptr}

{
        setObjectName("NfMainMenu");

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

        auto menuLayout = new QVBoxLayout(this);
        menuLayout->setContentsMargins(6, 6, 6, 6);
        menuLayout->setSpacing(2);

        shootsButton = new QPushButton(tr("Shoots"), this);
        connect(shootsButton, &QPushButton::clicked, this, &NfMainMenu::shootsClicked);
        menuLayout->addWidget(shootsButton);

        libraryButton = new QPushButton(tr("Library"), this);
        connect(libraryButton, &QPushButton::clicked, this, &NfMainMenu::libraryClicked);
        menuLayout->addWidget(libraryButton);

        foldersButton = new QPushButton(tr("Folders"), this);
        connect(foldersButton, &QPushButton::clicked, this, &NfMainMenu::foldersClicked);
        menuLayout->addWidget(foldersButton);

        menuLayout->addStretch();

        setLayout(menuLayout);
}

} // namespace NfDesktop
