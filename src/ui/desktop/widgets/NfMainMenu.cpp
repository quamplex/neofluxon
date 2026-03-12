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

namespace Desktop {

NfMainMenu::NfMainMenu(QWidget* parent)
        : NfStyledWidget(parent)
        , newShootButton{nullptr}
        , shootsButton{nullptr}
        , libraryButton{nullptr}
        , settingsButton{nullptr}

{
        setObjectName("NfMainMenu");

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

        auto menuLayout = new QVBoxLayout(this);
        menuLayout->setContentsMargins(6, 6, 6, 6);
        menuLayout->setSpacing(2);

        newShootButton = new QPushButton("📁 New Shoot", this);
        connect(newShootButton, &QPushButton::clicked, this, &NfMainMenu::newShootClicked);
        menuLayout->addWidget(newShootButton);

        shootsButton = new QPushButton("🖼️ Shoots", this);
        connect(shootsButton, &QPushButton::clicked, this, &NfMainMenu::shootsClicked);
        menuLayout->addWidget(shootsButton);

        libraryButton = new QPushButton("🗂️ Library", this);
        connect(libraryButton, &QPushButton::clicked, this, &NfMainMenu::libraryClicked);
        menuLayout->addWidget(libraryButton);

        settingsButton = new QPushButton("⚙️ Settings", this);
        connect(settingsButton, &QPushButton::clicked, this, &NfMainMenu::settingsClicked);
        menuLayout->addWidget(settingsButton);

        menuLayout->addStretch();

        setLayout(menuLayout);
}

} // namespace Desktop
