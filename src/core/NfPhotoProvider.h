/**
 * File name: NfPhotoProvider.h
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

#ifndef NF_THUMBNAIL_PROVIDER_H
#define NF_THUMBNAIL_PROVIDER_H

#include "Neofluxon.h"

namespace NfCore {

class NfCache;
class NfPathLoader;

class NfPhotoProvider {
public:
        using ItemNumberChangedCallback = std::function<void()>;
        using ThumbnailReadyCallback = std::function<void(size_t index)>;
        NfPhotoProvider();
        ~NfPhotoProvider();

        void requestThumbnail(const NfPhotoInfo &info);

        // Browsers callbacks
        void setItemNumberChangedCallback(ItemNumberChangedCallback callback);
        void setThumbnailReadyCallback(ThumbnailReadyCallback callback);

        void setPath(const std::filesystem::path &path);
        std::filesystem::path getPath() const;
        void setCache(NfCache *cache);
        NfCache* getCache() const;
        NfThumbnail getThumbnailAt(size_t index) const;
        size_t numberOfThumbnails() const;

 protected:

 private:
        ItemsNumberChangedCallback m_itemNumberChangedCallback;
        ThumbnailReadyCallback m_thumbnailReadyCallback;

        std::unique_ptr<NfPathLoader> m_pathLoader;
        NfCache* m_cache;
};

} // namespace NfCore

#endif // NF_THUMBNAIL_PROVIDER_H
