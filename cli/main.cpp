
/* ************************************************************************ */

// C++
#include <iostream>
#include <string>
#include <exception>
#include <csignal>
#include <atomic>
#include <mutex>

// Cell Sim
#include "simulator/Simulator.h"
#include "simulator/World.h"
#include "javascript/WorldFactory.h"

/* ************************************************************************ */

std::atomic_bool g_terminated{false};

/* ************************************************************************ */

// Simulator
simulator::Simulator g_sim;

/* ************************************************************************ */

/// Mutex for logging.
std::mutex g_log_mutex;

/* ************************************************************************ */

[[noreturn]] void error(const std::string& err)
{
    std::cerr << err << std::endl;
    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Stop simulation request.
 *
 * @param param
 */
void stop_simulation(int param)
{
    g_sim.stop();
    g_terminated = true;
}

/* ************************************************************************ */

/**
 * @brief Entry function.
 *
 * @param argc
 * @param argv
 */
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        error("not enough arguments: <script.js>");
    }

    try
    {
        // Create javascript world factory
        javascript::WorldFactory worldFactory;

        // Create world
        g_sim.setWorld(worldFactory.createWorldFromFile(argv[1]));

        g_sim.getWorld()->setLogFunction([](const std::string& msg) {
            std::lock_guard<std::mutex> _{g_log_mutex};
            std::cout << msg << "\n";
        });

        // Install signal handler
        signal(SIGINT, stop_simulation);

        // Start simulation
        g_sim.start();

        if (g_terminated)
            std::cout << "Simulation terminated" << std::endl;
        else
            std::cout << "Simulation finished" << std::endl;
    }
    catch (const std::exception& e)
    {
        error(e.what());
    }
}

/* ************************************************************************ */
