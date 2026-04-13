/**
 * File name: NfUiState.cpp
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

#include "NfUiState.h"
#include "NfUiShootsModeState.h"
#include "NfUiFolderModeState.h"
#include "NfUiLibraryModeState.h"

namespace NfUi {

NfUiState::NfUiState(QObject* parent)
        : QObject(parent)
        , m_uiMode{NfUiMode::Shoots}
        , m_shootsModeState{new NfUiShootsModeState(this)}
        , m_folderModeState{new NfUiFolderModeState(this)}
        , m_libraryModeState{new NfUiLibraryModeState(this)}
{
}

NfUiFolderModeState* NfUiState::shootsModeState() const
{
        return m_shootsModeState;
}

NfUiFolderModeState* NfUiState::folderModeState() const
{
        return m_folderModeState;
}

NfUiLibraryModeState* NfUiState::libraryModeState() const
{
        return m_libraryModeState;
}

NfUiMode NfUiState::mode() const
{
        return m_uiMode;
}

void NfUiState::setMode(NfUiMode mode)
{
        if (m_uiMode == mode)
                return;

        m_uiMode = mode;
        emit modeChanged(m_uiMode);
}

} // namespace NfUi
