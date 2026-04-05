#ifndef NF_GUI_CACHE_H
#define NF_GUI_CACHE_H

#include "NfPhotoId.h"

#include <memory>
#include <optional>
#include <list>
#include <unordered_map>
#include <cstddef>

namespace NfCore {

//class NfDiskCache;
class NfImage;

/**
 * @brief GUI image cache with LRU eviction and optional disk overflow.
 *
 * Stores images in memory up to a size limit. When exceeded, least-recently-used
 * items are evicted. Evicted items can optionally be stored in a disk cache.
 *
 * Accessing or updating an item marks it as recently used.
 *
 * @warning Not thread-safe. Must be used only from the GUI thread.
 */
class NfGuiCache {
public:
    static constexpr std::size_t DEFAULT_MAX_SIZE_BYTES = 250ULL * 1024 * 1024;

    explicit NfGuiCache(/*NfDiskCache* diskCache = nullptr,*/
                        std::size_t maxSizeBytes = DEFAULT_MAX_SIZE_BYTES);
    ~NfGuiCache();

    NfGuiCache(const NfGuiCache&) = delete;
    NfGuiCache& operator=(const NfGuiCache&) = delete;

    NfGuiCache(NfGuiCache&&) noexcept = default;
    NfGuiCache& operator=(NfGuiCache&&) noexcept = default;

    /// Adds or replaces an image. Updates LRU order.
    /// Image is rejected if it exceeds max cache size.
    void add(const NfPhotoId &id, std::unique_ptr<NfGuiImage> image);

    // Returns image if present and marks it as recently used.
    /// @warning Returned pointer may become invalid after any cache modification.
    [[nodiscard]] NfGuiImage* get(const NfPhotoId& id);

    /// Removes image if present.
    [[nodiscard]] bool remove(const NfPhotoId& id);

    /// Clears all cached images.
    void clear();

    [[nodiscard]] std::size_t currentSizeBytes() const noexcept;
    [[nodiscard]] std::size_t maxSizeBytes() const noexcept;

    /// Updates max cache size and evicts items if necessary.
    void setMaxSizeBytes(std::size_t maxSizeBytes);

private:
    /// Marks item as recently used.
    void refreshAccess(std::list<NfPhotoId>::iterator it);

    /// Removes id from LRU list.
    void removeFromLRU(std::list<NfPhotoId>::iterator it);

    /// Evicts least-recently-used items until required space is available.
    void evictUntilFits(std::size_t requiredSize);

private:
    //    NfDiskCache* m_diskCache;
    std::size_t m_maxSizeBytes;
    std::size_t m_currentSizeBytes;
    std::unordered_map<
        NfPhotoId,
        std::pair<std::list<NfPhotoId>::iterator,
        std::unique_ptr<NfImage>>> m_memoryCache;
    std::list<NfPhotoId> m_lruOrder; // front = most recent
};

} // namespace NfCore

#endif // NF_GUI_CACHE_H
