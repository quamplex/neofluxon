#include "NfFolderModel.h"
#include "NfBrowserModel.h"

namespace NfDesktop {

NfFolderModel::NfFolderModel(const NfContext *ctx, QObject *parent)
        : QObject(parent)
        , m_context{ctx}
        , m_browserModel{new NfBrowserModel(m_context, this)}
{
}

NfBrowserModel* NfFolderModel::browser() const
{
        return m_browserModel;
}

} // namespace NfDesktop
