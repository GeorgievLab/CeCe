/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include <chrono>
#ifdef __MINGW32__
// MinGW has issue with undefined reference to __impl_nanosleep (nanosleep function)
#undef _GLIBCXX_USE_NANOSLEEP
#endif
#include <thread>

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

    // Update simulation
    update(units::ms(16));

    // Sleep
    std::this_thread::sleep_until(sleep);
}

/* ************************************************************************ */

bool Simulator::update(units::Time dt)
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
void Simulator::draw(unsigned int width, unsigned int height)
{
    // Delete old objects
    m_renderContext.deleteReleasedObjects();

    // Start frame
    m_renderContext.frameBegin(width, height);

    assert(m_simulation);
    m_simulation->draw(m_renderContext);

    m_renderContext.frameEnd();
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
