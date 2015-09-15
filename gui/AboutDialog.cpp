/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Definition
#include "AboutDialog.h"

// wxWidgets
#include <wx/app.h>

// GUI
#include "version.hpp"

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

