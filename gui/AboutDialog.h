/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// GUI
#include "wxcrafter.h"

/* ************************************************************************ */

/**
 * @brief Application about dialog.
 */
class AboutDialog : public AboutDialogBaseClass
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent Parent window.
     */
    explicit AboutDialog(wxWindow* parent);

};

/* ************************************************************************ */

