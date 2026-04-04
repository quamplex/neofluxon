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

        std::unique_ptr<NfThumbail> takeThumbnail() const;

        [[nodiscard]] std::string getErrorMessage() const { return m_errorMessage; }

private:
        NfPhoto m_photo;
        std::unique_ptr<NfImage> m_imageContainer;
        std::string m_errorMessage;
};
