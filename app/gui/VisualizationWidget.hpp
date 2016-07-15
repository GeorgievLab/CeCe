/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

#pragma once

/* ************************************************************************ */

// Qt
#include <QGLWidget>
#include <QScopedPointer>
#include <QPoint>

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/render/Context.hpp"
#include "cece/simulator/Simulation.hpp"

// GUI
#include "Simulator.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief OpenGL visualization widget.
 */
class VisualizationWidget : public QGLWidget
{
    Q_OBJECT

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     */
    VisualizationWidget(QWidget* parent = nullptr);


// Public Accessors
public:


    /**
     * @brief Return simulator.
     *
     * @return
     */
    ViewPtr<Simulator> getSimulator() const noexcept
    {
        return m_simulator;
    }


// Public Mutators
public:


    /**
     * @brief Change simulator.
     *
     * @return
     */
    void setSimulator(ViewPtr<Simulator> simulator) noexcept
    {
        m_simulator = simulator;
    }


// Public Operations
public:


    /**
     * @brief Fit simulation to view.
     */
    void fitToView() noexcept;


// Protected Events
protected:


    /**
     * @brief Close event.
     *
     * @param event
     */
    void closeEvent(QCloseEvent* event) override;


    /**
     * @brief Mouse scroll event.
     *
     * @param event
     */
    void wheelEvent(QWheelEvent* event) override;


    /**
     * @brief Mouse press event.
     *
     * @param event
     */
    void mousePressEvent(QMouseEvent* event) override;


    /**
     * @brief Mouse release event.
     *
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent* event) override;


    /**
     * @brief Mouse move event.
     *
     * @param event
     */
    void mouseMoveEvent(QMouseEvent* event) override;


    /**
     * @brief Initialize OpenGL.
     */
    void initializeGL() override;


    /**
     * @brief Resize OpenGL view.
     *
     * @param width
     * @param height
     */
    void resizeGL(int width, int height) override;


    /**
     * @brief Paint OpenGL.
     */
    void paintGL() override;


// Private Data Members
private:

    /// Simulator.
    ViewPtr<Simulator> m_simulator;

    /// OpenGL context.
    ViewPtr<QOpenGLContext> m_context;

    /// Rendering context.
    render::Context m_renderContext;

    /// Mouse position.
    QPoint m_mousePos;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
