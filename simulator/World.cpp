
/* ************************************************************************ */

// Declaration
#include "World.h"

// C++
#include <algorithm>

// Library
#include "Cell.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World(std::unique_ptr<WorldImplData> data) noexcept
    : m_implData(std::move(data))
{
    // Nothing to do
}

/* ************************************************************************ */

World::~World()
{
    // Cells must be destroyed before the physics world
    m_cells.clear();
}

/* ************************************************************************ */

void World::update() noexcept
{
    physics::World::Update();
}

/* ************************************************************************ */

void World::draw(render::Context& context) const noexcept
{
    /*
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(5, 5, 5);

    glBegin(GL_LINES);

    glColor3d(1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);

    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);

    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 1.0);

    glEnd();
*/
    for (const auto& cell : m_cells)
    {
        assert(cell);
        cell->draw(context);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */
