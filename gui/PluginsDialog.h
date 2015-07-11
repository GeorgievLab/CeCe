/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
 * @brief Plugins dialog.
 */
class PluginsDialog : public PluginsDialogBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     */
    explicit PluginsDialog(wxWindow* parent);


// Protected Events
protected:


    /**
     * @brief On dialog initialize event.
     *
     * @param event
     */
    void OnInitDialog(wxInitDialogEvent& event) override;

};

/* ************************************************************************ */
