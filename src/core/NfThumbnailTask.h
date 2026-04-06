/**
 * File name: NfThumbnailTask.h
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

#ifndef NF_THUMBNAIL_H
#define NF_THUMBNAIL_H

#include "NfTask.h"
#include "NfPhoto.h"

#include <memory>
#include <string>
#include <cstdint>

namespace NfCore {

class NfImage;
class NfThumbnail;

class NfThumbnailTask : public NfTask {
public:
        NfThumbnailTask(const NfPhoto& photo, std::unique_ptr<NfImage> imageContainer);
        ~NfThumbnailTask();

        void setGenerationId(uint64_t generationId);
        uint64_t generationId() const;

        TaskStatus execute() override;

        std::unique_ptr<NfThumbnail> takeThumbnail();

        [[nodiscard]] std::string getErrorMessage() const { return m_errorMessage; }

private:
        uint64_t m_generationId;
        NfPhoto m_photo;
        std::unique_ptr<NfImage> m_imageContainer;
        std::string m_errorMessage;
};

} // namespace NfCore

#endif // NF_THUMBNAIL_H
