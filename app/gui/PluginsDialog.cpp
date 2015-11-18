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

// Declaration
#include "PluginsDialog.h"

// Simulator
#include "simulator/PluginManager.hpp"

/* ************************************************************************ */

PluginsDialog::PluginsDialog(wxWindow* parent)
    : PluginsDialogBase(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void PluginsDialog::OnInitDialog(wxInitDialogEvent& event)
{
    for (auto&& name : simulator::PluginManager::getNames())
    {
        m_listBoxPlugins->Append(wxString(name));
    }

    const auto& directories = simulator::PluginManager::getDirectories();

    if (!directories.empty())
        m_staticTextPath->SetLabel(directories[0]);
}

/* ************************************************************************ */
