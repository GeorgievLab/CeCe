
// Definition
#include "AboutDialog.h"

// wxWidgets
#include <wx/app.h>

// Simulator
#include "simulator/version.hpp"

/* ************************************************************************ */

static wxString buildString()
{
    wxString res = wxVERSION_STRING;

#if defined(__i386__) || defined(_M_IX86) || defined(_X86_)
    res << " x86";
#elif defined(__amd64__) || defined(__x86_64__) || defined(_M_AMD64)
    res << " x64";
#endif

    // http://sourceforge.net/p/predef/wiki/Compilers/
#if defined(__clang__)
    res << " (Clang)";
#elif defined(__GNUC__)
    res << " (GNU C++)";
#elif defined(__INTEL_COMPILER)
    res << " (Intel C++)";
#elif defined(__MINGW32__)
    res << " (MinGW)";
#elif defined(__MINGW64__)
    res << " (MinGW-w64)";
#elif defined(_MSC_VER)
    res << " (Microsoft Visual C++)";
#endif

    return res;
}

/* ************************************************************************ */

AboutDialog::AboutDialog(wxWindow* parent)
    : AboutDialogBaseClass(parent)
{
//    {
//        auto font = m_staticTextAppName->GetFont();
//        font.SetPointSize(20);
//        m_staticTextAppName->SetFont(font);
//    }

    m_staticTextAppName->SetLabel(wxTheApp->GetAppDisplayName());
    m_staticTextVersion->SetLabel(_("version ") + VERSION);
    m_staticTextCopyright->SetLabel("Copyright 2015 Georgiev Lab");
    m_staticTextBuild->SetLabel(buildString());
}

/* ************************************************************************ */

