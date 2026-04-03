#pragma once

#include "NfTask.h"
#include <memory>
#include <string>

// Forward declarations
class NfPhoto;

class NfThumbnailTask : public NfTask {
public:
        NfThumbnailTask(const NfPhoto& photo);

        // Destructor
        ~NfThumbnailTask() override = default;

        TaskStatus execute() override;

        [[nodiscard]] std::unique_ptr<NfImage> takeImage();

    /**
     * @brief Gets the source photo data.
     */
    [[nodiscard]] const NfPhoto& getPhoto() const { return m_photo; }

    /**
     * @brief Gets the last error message if execution failed.
     */
    [[nodiscard]] std::string getErrorMessage() const { return m_errorMessage; }

private:
    NfPhoto m_photo;
    std::unique_ptr<NfGuiImage> m_imageContainer;
    std::string m_errorMessage;
};
