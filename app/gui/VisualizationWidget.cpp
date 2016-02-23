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

// Declaration
#include "VisualizationWidget.hpp"

// Qt
#include <QWheelEvent>

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

VisualizationWidget::VisualizationWidget(QWidget* parent)
    : QGLWidget(parent)
{
    // Nothing to do
}

/* ************************************************************************ */

void VisualizationWidget::wheelEvent(QWheelEvent* event)
{
    constexpr float ZOOM_COEFF = 1.1;

    // Get change
    const auto offset = event->angleDelta();

    // Get camera
    auto& camera = m_renderContext.getCamera();

    if (offset.y() < 0)
        camera.setZoom(camera.getZoom() * ZOOM_COEFF);
    else if (offset.y() > 0)
        camera.setZoom(camera.getZoom() * 1.0 / ZOOM_COEFF);

    // Update widget
    update();
}

/* ************************************************************************ */

void VisualizationWidget::initializeGL()
{
    m_renderContext.init();
}

/* ************************************************************************ */

void VisualizationWidget::resizeGL(int width, int height)
{
    m_renderContext.setView(width, height);
}

/* ************************************************************************ */

void VisualizationWidget::paintGL()
{
    // Delete old objects
    m_renderContext.deleteReleasedObjects();

    // Start frame
    m_renderContext.frameBegin(width(), height());

    if (m_simulation)
        m_simulation->draw(m_renderContext);

    m_renderContext.frameEnd();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
