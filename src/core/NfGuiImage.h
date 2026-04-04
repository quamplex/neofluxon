#ifndef NF_GUI_IMAGE_H
#define NF_GUI_IMAGE_H

#include <cstddef>

namespace NfCore {

class NfImageData;

/**
 * @brief Abstract GUI image representation.
 *
 * Provides a platform-independent interface for storing and accessing
 * image data in the GUI layer.
 *
 * Implementations define how image data is stored (e.g. QPixmap in Qt).
 */
class NfGuiImage : public NfImage {
public:
        virtual ~NfGuiImage() = default;
        virtual void setImageData(const NfImageData& data) override;
        virtual std::size_t size() const = 0;
};

} // namespace NfCore

#endif // NF_GUI_IMAGE_H
