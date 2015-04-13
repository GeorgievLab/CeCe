
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

Simulator::~Simulator()
{
    stop();
}

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

void Simulator::stop()
{
    m_isRunning = false;
}

/* ************************************************************************ */

void Simulator::step()
{
    // When to wake-up
    auto sleep = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(16);

    {
        update(0.016f);
    }

    // Sleep
    std::this_thread::sleep_until(sleep);
}

/* ************************************************************************ */

void Simulator::reset()
{
    assert(m_simulation);
    m_simulation->reset();
}

/* ************************************************************************ */

void Simulator::update(units::Duration dt)
{
    assert(m_simulation);
    m_simulation->update(dt);
}

/* ************************************************************************ */

void Simulator::update()
{
    assert(m_simulation);
    update(m_simulation->getTimeStep());
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulator::drawInit(render::Context& context)
{
    assert(m_simulation);
    m_simulation->drawInit(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulator::draw(render::Context& context)
{
    assert(m_simulation);
    m_simulation->draw(context);
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
