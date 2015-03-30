
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/spinctrl.h>

// Simulator
#include "gui/VectorCtrlBase.h"

/* ************************************************************************ */

/**
 * @brief Control for integer vector.
 */
class VectorIntCtrl : public VectorCtrlBase<wxSpinCtrl, int>
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     * @param id
     */
    VectorIntCtrl(wxWindow* parent, wxWindowID id)
        : VectorCtrlBase<wxSpinCtrl, int>(parent, id)
    {
        // Nothing to do
    }

};

/* ************************************************************************ */
