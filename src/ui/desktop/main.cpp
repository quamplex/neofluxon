/**
 * File name: main.cpp
 * Project: Neofluxon (A photography workflow software)
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

#include "NfCore.h"
#include "LqfMainWindow.h"
#include "LqfApplication.h"

#include <QFile>

using namespace NfDesktop;

class NfQtPixmap : NfImageObject {
public:
        QtPixmapChaceObject(const NfImage* image) {
                QImage img(image->data(),
                           image->width(),
                           image->height(),
                           image->stride(),
                           QImage::Format_RGBA8888);
                m_pixmap = QPixmap::fromImage(img.copy());
        }

private:
        m_pixmap;
};


int main(int argc, char **argv)
{
        LqfApplication app(argc, argv);

        LqfMainWindow mainWin();
        mainWin.show();


        return app.exec();
}
