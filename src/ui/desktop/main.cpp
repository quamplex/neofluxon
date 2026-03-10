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

#include "LqfMainWindow.h"
#include "LqfApplication.h"

using namespace Desktop;

int main(int argc, char **argv)
{
        LqfApplication app(argc, argv);

        QString globalStyle = R"(
/* Set the base font and text color for all widgets */
* {
    font-family: "Roboto", "Segoe UI", "Arial", sans-serif;
    font-size: 10pt;
    color: #444;  /* dark gray text */
}

/* QMainWindow background */
QMainWindow, QCentralWidget, PhotoEditor {
    background-color: #191820;
}

QWidget {
    background-color: #23232b;
}



)";

        //        app.setStyleSheet(globalStyle);


        LqfMainWindow mainWin;
        mainWin.show();
        return app.exec();
}
