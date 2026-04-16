#include "NfFolderModel.h"
#include "NfBrowserModel.h"

namespace NfDesktop {

NfFolderModel::NfFolderModel(const NfFolderModeContext &ctx, QObject *parent)
        : QObject(parent)
        , m_context{ctx}
        , m_browserModel{new NfBrowserModel(m_context.photoProvider(), this)}
{
}

NfBrowserModel* NfFolderModel::browser() const
{
        return m_browserModel;
}

} // namespace NfDesktop
