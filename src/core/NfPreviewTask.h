/**
 * File name: NfPreviewTask.h
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

#ifndef NF_PREVIEW_TASK_H
#define NF_PREVIEW_TASK_H

#include "NfTask.h"
#include "NfPhoto.h"
#include "NfPreview.h"

#include <memory>
#include <cstdint>

namespace NfCore {

class NfImage;

class NfPreviewTask : public NfTask {
public:
        using ImageSource = NfPreview::ImageSource;

        NfPreviewTask(const NfPhoto& photo, std::unique_ptr<NfImage> imageContainer);
        ~NfPreviewTask();

        void setGenerationId(uint64_t generationId);
        uint64_t generationId() const;

        void setImageSource(ImageSource source);
        ImageSource imageSource() const;

        TaskStatus execute() override;
        std::unique_ptr<NfPreview> takePreview();

private:
        uint64_t m_generationId;
        NfPhoto m_photo;
        std::unique_ptr<NfImage> m_imageContainer;
        ImageSource m_imageSource;
};

} // namespace NfCore

#endif // NF_PREVIEW_TASK_H
