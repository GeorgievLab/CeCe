
#pragma once

/* ************************************************************************ */

// wxWidgets
#include <wx/spinctrl.h>

// Simulator
#include "gui/VectorCtrlBase.h"

/* ************************************************************************ */

using VectorFloatCtrlEvent = VectorCtrlEventBase<float>;

/* ************************************************************************ */

wxDECLARE_EVENT(EVT_VECTOR_FLOAT_CTRL_UPDATED, VectorFloatCtrlEvent);

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
        m_valueX->Bind(wxEVT_SPINCTRLDOUBLE, &VectorFloatCtrl::OnChange, this);
        m_valueY->Bind(wxEVT_SPINCTRLDOUBLE, &VectorFloatCtrl::OnChange, this);
    }


// Public Operations
public:


    /**
     * @brief One of the values has been changed.
     *
     * @param event
     */
    void OnChange(wxSpinDoubleEvent& event)
    {
        VectorFloatCtrlEvent evt;
        evt.SetValue(GetValue());
        wxPostEvent(GetParent(), evt);
    }
};

/* ************************************************************************ */

using VectorFloatCtrlEventFunction = void (wxEvtHandler::*)(VectorFloatCtrlEvent&);

/* ************************************************************************ */

#define VectorFloatCtrlEventHandler(func) \
    wxEVENT_HANDLER_CAST(VectorFloatCtrlEventFunction, func)

/* ************************************************************************ */
