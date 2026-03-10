/**
 * File name: LqfPanel.cpp
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

#include "LqfPanel.h"

#include <QWidget>
#include <QGraphicsDropShadowEffect>

namespace Desktop {

LqfPanel::LqfPanel(QWidget *parent, PanelPosition position)
        : QFrame(parent)
        , panelPosition{position}
{
        setAttribute(Qt::WA_StyledBackground, true);
        setProperty("side", panelPosition == PanelPosition::Left ? "left" : "right");

        //        setStyleSheet("  background-color: #fff;"
        //                     );

        /*auto shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(15);
        shadow->setColor(QColor(0, 0, 0,
                                panelPosition == PanelPosition::Left ? 35 : 30));
        shadow->setOffset(panelPosition == PanelPosition::Left ? 3 : -3, 0);
        setGraphicsEffect(shadow);*/
}

} // namespace Desktop

