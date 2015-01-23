
/* ************************************************************************ */

// Declaration
#include "simulator/Simulator.h"

// C++
#include <cassert>
#include <thread>
#include <chrono>

// Simulator
#include "simulator/World.h"

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
    auto sleep = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1000 / 60);

    {
        assert(m_world);
        m_world->update(1 / 60.f);
    }

    // Sleep
    std::this_thread::sleep_until(sleep);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
