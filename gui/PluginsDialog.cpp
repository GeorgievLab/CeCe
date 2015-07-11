/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "PluginsDialog.h"

// Simulator
#include "simulator/Plugin.hpp"

/* ************************************************************************ */

PluginsDialog::PluginsDialog(wxWindow* parent)
    : PluginsDialogBase(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void PluginsDialog::OnInitDialog(wxInitDialogEvent& event)
{
    for (auto&& name : simulator::Plugin::getNames())
    {
        m_listBoxPlugins->Append(wxString(name));
    }

    const auto& directories = simulator::Plugin::getDirectories();

    if (!directories.empty())
        m_staticTextPath->SetLabel(directories[0]);
}

/* ************************************************************************ */
