
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/spinctrl.h>

// Simulator
#include "gui/VectorCtrlBase.h"

/* ************************************************************************ */

using VectorIntCtrlEvent = VectorCtrlEventBase<int>;

/* ************************************************************************ */

wxDECLARE_EVENT(EVT_VECTOR_INT_CTRL_UPDATED, VectorIntCtrlEvent);

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
        m_valueX->Bind(wxEVT_SPINCTRL, &VectorIntCtrl::OnChange, this);
        m_valueY->Bind(wxEVT_SPINCTRL, &VectorIntCtrl::OnChange, this);
    }


// Public Operations
public:


    /**
     * @brief One of the values has been changed.
     *
     * @param event
     */
    void OnChange(wxSpinEvent& event)
    {
        VectorIntCtrlEvent evt;
        evt.SetValue(GetValue());
        wxPostEvent(GetParent(), evt);
    }
};

/* ************************************************************************ */

using VectorIntCtrlEventFunction = void (wxEvtHandler::*)(VectorIntCtrlEvent&);

/* ************************************************************************ */

#define VectorIntCtrlEventHandler(func) \
    wxEVENT_HANDLER_CAST(VectorIntCtrlEventFunction, func)

/* ************************************************************************ */
