
/* ************************************************************************ */

// Declaration
#include "simulator/Simulator.hpp"

// C++
#include <cassert>
#include <thread>
#include <chrono>

// Simulator
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Simulator::Simulator()
{
    // Nothing
}

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
        assert(m_world);
        m_world->update();
    }

    // Sleep
    std::this_thread::sleep_until(sleep);
}

/* ************************************************************************ */

void Simulator::reset()
{
    assert(m_world);
    m_world->reset();
}

/* ************************************************************************ */

void Simulator::update(units::Duration dt)
{
    assert(m_world);
    m_world->update(dt);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void Simulator::render(render::Context& context)
{
    assert(m_world);
    m_world->render(context);
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
