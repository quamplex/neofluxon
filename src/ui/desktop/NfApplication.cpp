/**
 * File name: NfApplication.cpp
 * Project: LiquidFlow (a photography workflow software)
 *
 * Copyright (C) 2026 Iurie Nistor
 *
 * This file is part of LiquidFlow.
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

#include "NfApplication.h"
#include "core/NeofluxonCore.h"
#include "NfPhotoProvider.h"
#include "core/NfLogger.h"

#include <QProcessEnvironment>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>

using namespace NfCore;

namespace NfDesktop {

        NfApplication::NfApplication(NeofluxonCore* coreApp,
                                       int &argc,
                                       char **argv,
                                       int falgs)
        : QApplication(argc, argv, falgs)
        , m_photoProvider{new NfPhotoProvider(coreApp->photoLoader(), coreApp->guiCache(), this)}
{
        QFile styleFile(":/style-cool-teal-desktop.qcss");  // matches the prefix "/" + file name
        if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString style = QString::fromUtf8(styleFile.readAll());
                setStyleSheet(style);  // applies to entire application
        } else {
                qDebug() << "Failed to open style-desktop.qcss!";
        }
}

NfApplication::~NfApplication()
{
        NF_LOG_DEBUG("called");
}

NfApplication* NfApplication::getAppInstance()
{
        return static_cast<NfApplication*>(QApplication::instance());
}

QString NfApplication::applicationName()
{
        return "LiquidFlow";;
}

NfPhotoProvider& NfApplication::photoProvider() const
{
        return *m_photoProvider;
}

} // namespace NfDesktop
