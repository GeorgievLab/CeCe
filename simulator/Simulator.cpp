/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/Simulator.hpp"

// C++
#include <cassert>
#include <thread>
#include <chrono>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

void Simulator::start()
{
    m_isRunning = true;

    while (m_isRunning)
    {
        step();
    }
}

/* ************************************************************************ */

void Simulator::step()
{
    // When to wake-up
    auto sleep = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(16);

    {
        update(units::ms(16));
    }

    // Sleep
    std::this_thread::sleep_until(sleep);
}

/* ************************************************************************ */

bool Simulator::update(units::Duration dt)
{
    assert(m_simulation);
    return m_simulation->update(dt);
}

/* ************************************************************************ */

bool Simulator::update()
{
    assert(m_simulation);
    return m_simulation->update();
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Simulator::draw(unsigned width, unsigned height)
{
    // Delete old object
    m_renderContext.deleteReleasedObjects();

    m_renderContext.frameBegin(width, height);

    assert(m_simulation);
    m_simulation->draw(m_renderContext);

    m_renderContext.frameEnd();
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
