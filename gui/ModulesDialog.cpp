/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "ModulesDialog.h"

// Simulator
#include "simulator/Plugin.hpp"

/* ************************************************************************ */

ModulesDialog::ModulesDialog(wxWindow* parent)
    : ModulesDialogBase(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void ModulesDialog::OnInitDialog(wxInitDialogEvent& event)
{
    for (auto&& name : simulator::Plugin::getNames())
    {
        m_listBoxModules->Append(wxString(name));
    }
}

/* ************************************************************************ */
