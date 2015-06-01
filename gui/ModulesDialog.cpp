/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "ModulesDialog.h"

// Simulator
#include "simulator/Library.hpp"

/* ************************************************************************ */

ModulesDialog::ModulesDialog(wxWindow* parent)
    : ModulesDialogBase(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void ModulesDialog::OnInitDialog(wxInitDialogEvent& event)
{
    for (auto&& name : simulator::Library::getBuildInNames())
    {
        m_listBoxModules->Append(wxString(name));
    }
}

/* ************************************************************************ */
