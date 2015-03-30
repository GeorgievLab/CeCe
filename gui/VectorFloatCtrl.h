
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
class VectorFloatCtrl : public VectorCtrlBase<wxSpinCtrlDouble, float>
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     * @param id
     */
    VectorFloatCtrl(wxWindow* parent, wxWindowID id)
        : VectorCtrlBase<wxSpinCtrlDouble, float>(parent, id)
    {
        // Nothing to do
    }

};

/* ************************************************************************ */
