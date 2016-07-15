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

// Declaration
#include "VisualizationWidget.hpp"

// Qt
#include <QCloseEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMutexLocker>

// CeCe
#include "cece/core/Units.hpp"
#include "cece/simulator/Visualization.hpp"

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

void VisualizationWidget::fitToView() noexcept
{
    if (!m_simulator && m_simulator->getSimulation())
        return;

    auto size = m_simulator->getSimulation()->getWorldSize();
    auto& camera = m_renderContext.getCamera();
    camera.setPosition(Zero);
    camera.setZoom(
        std::max(size.getWidth() / width(), size.getHeight() / height()).value()
    );
}

/* ************************************************************************ */

void VisualizationWidget::closeEvent(QCloseEvent* event)
{
    makeCurrent();

    // Delete old objects
    m_renderContext.deleteReleasedObjects();
    m_renderContext.deleteObjects();
}

/* ************************************************************************ */

void VisualizationWidget::wheelEvent(QWheelEvent* event)
{
    Q_ASSERT(event);

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

void VisualizationWidget::mousePressEvent(QMouseEvent* event)
{
    Q_ASSERT(event);

    // Store position
    if (event->button() == Qt::LeftButton)
        m_mousePos = event->pos();

    QGLWidget::mousePressEvent(event);
}

/* ************************************************************************ */

void VisualizationWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QGLWidget::mouseReleaseEvent(event);
}

/* ************************************************************************ */

void VisualizationWidget::mouseMoveEvent(QMouseEvent* event)
{
    Q_ASSERT(event);

    // Get camera
    auto& camera = m_renderContext.getCamera();

    // Get position
    auto pos = camera.getPosition();

    // Change vector
    const auto change = event->pos() - m_mousePos;

    pos += camera.getZoom() * units::PositionVector{
        units::Length(change.x()),
        units::Length(-change.y())
    };

    camera.setPosition(pos);

    // Update mouse position
    m_mousePos = event->pos();

    QGLWidget::mouseMoveEvent(event);
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

    if (m_simulator && m_simulator->getSimulation() && m_simulator->getSimulation()->isInitialized())
    {
        // Set clear color
        m_renderContext.setClearColor(m_simulator->getSimulation()->getVisualization().getBackgroundColor());

        // TODO: remove - slow in case of expensive simulation updates
        QMutexLocker _(m_simulator->getMutex());
        m_simulator->getSimulation()->draw(m_renderContext);
    }
    else
    {
        // There is a weird bug (Intel GPU?) which doesn't clear widget content
        // So something must be drawn to fix it
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0, 0);
        glEnd();
    }

    m_renderContext.frameEnd();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
