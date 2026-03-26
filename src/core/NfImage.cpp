#include "NfImage.h"

NfImage::NfImage()
        : m_data(std::make_unique<NfImageData>())
{
}

NfImage::NfImage(std::unique_ptr<NfImageData> data)
        : m_data{std::move(data)}
{
}

NfImageData* NfImage::getData()
{
    return m_data.get();
}

const NfImageData* NfImage::getData() const
{
    return m_data.get();
}

int NfImage::width() const
{
    return m_data ? m_data->width() : 0;
}

int NfImage::height() const
{
    return m_data ? m_data->height() : 0;
}

int NfImage::channels() const
{
        return m_data ? m_data->channels() : 0;
}

std::string_view NfImage::format() const
{
        return "Unknown";
}

bool NfImage::isValid() const
{
        return !m_data->empty();
}
