/**
 * File name: LqfApplication.cpp
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

#include "LqfApplication.h"

#include <QProcessEnvironment>
#include <QFile>
#include <QDebug>

namespace Desktop {

LqfApplication::LqfApplication(int &argc, char **argv, int falgs)
        : QApplication(argc, argv, falgs)
{
        auto stylePath = QCoreApplication::applicationDirPath()
                + "/resources/style-desktop.qcss";
        qDebug() << "stylePath: " << stylePath;

        QFile styleFile(stylePath);
        if(styleFile.open(QIODevice::ReadOnly)) {
                QString style = QLatin1String(styleFile.readAll());
                qApp->setStyleSheet(style);
                qDebug() << "style: " << style;
        }
}

LqfApplication* LqfApplication::getAppInstance()
{
        return static_cast<LqfApplication*>(QApplication::instance());
}

QString LqfApplication::applicationName()
{
        return "LiquidFlow";;
}

} // namespace Desktop
