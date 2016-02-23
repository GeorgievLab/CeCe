/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/render/Context.hpp"
#include "cece/simulator/Simulation.hpp"

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
     * @brief Return currently visualized simulation.
     *
     * @return
     */
    ViewPtr<simulator::Simulation> getSimulation() const noexcept
    {
        return m_simulation;
    }


// Public Mutators
public:


    /**
     * @brief Change currently visualized simulation.
     *
     * @return
     */
    void setSimulation(ViewPtr<simulator::Simulation> simulation) noexcept
    {
        m_simulation = simulation;
    }


// Protected Events
protected:


    /**
     * @brief Mouse scroll event.
     *
     * @param event
     */
    void wheelEvent(QWheelEvent* event) override;


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

    /// Drawn simulation.
    ViewPtr<simulator::Simulation> m_simulation;

    /// OpenGL context.
    ViewPtr<QOpenGLContext> m_context;

    /// Rendering context.
    render::Context m_renderContext;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
