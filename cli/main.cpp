
/* ************************************************************************ */

// C++
#include <iostream>
#include <string>
#include <exception>
#include <csignal>
#include <atomic>
#include <mutex>
#include <thread>
#include <fstream>
#ifdef ENABLE_RENDER
#include <chrono>
#endif

#ifdef ENABLE_RENDER
#include <GL/glut.h>
#endif

// Simulator
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/Module.hpp"
#include "parser-xml/SimulationFactory.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

#if ENABLE_PHYSICS_DEBUG
#include "Render.hpp"
#endif

/* ************************************************************************ */

std::atomic_bool g_terminated{false};

/* ************************************************************************ */

bool g_paused{false};

/* ************************************************************************ */

// Simulator
simulator::Simulator g_sim;

/* ************************************************************************ */

/// Mutex for logging.
std::mutex g_log_mutex;

/* ************************************************************************ */

#ifdef ENABLE_RENDER
GLuint g_width = 800;
GLuint g_height = 600;
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
using clock_type = std::chrono::high_resolution_clock;
clock_type::time_point g_start;
#endif

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
        error("not enough arguments: <desc.xml>");
    }

#if ENABLE_RENDER
    glutInit(&argc, argv);
    glutInitWindowSize(g_width, g_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_STENCIL);
    glutCreateWindow("Cell Simulator");

    g_sim.drawInit();
#endif

    std::ofstream time_file("time.csv");

    try
    {
        setMeasureTimeOutput(&time_file);

        // Create javascript world factory
        parser::xml::SimulationFactory simFactory;

        // Create world
        g_sim.setSimulation(simFactory.fromFile(argv[1]));

        // Get simulation
        auto simulation = g_sim.getSimulation();

#if ENABLE_RENDER && ENABLE_PHYSICS_DEBUG
        DebugDraw debugDraw;
        debugDraw.SetFlags(DebugDraw::e_shapeBit | DebugDraw::e_centerOfMassBit);
        simulation->getWorld().SetDebugDraw(&debugDraw);
#endif

#ifdef ENABLE_RENDER
        // Register callbacks:
        glutDisplayFunc([]() {
            g_sim.draw(g_width, g_height);
            glutSwapBuffers();
        });

        glutReshapeFunc([](int width, int height) {
            g_width = width;
            g_height = height;
            auto size = g_sim.getSimulation()->getWorldSize();

            g_sim.getRenderContext().getCamera().setZoom(
                std::max(size.getWidth() / g_width, size.getHeight() / g_height).value()
            );
        });

        // Idle function: update scene
        glutIdleFunc([]() {
            if (g_paused)
                return;

            auto start = clock_type::now();
            auto diff = start - g_start;
            float dt = std::chrono::duration<float, std::chrono::seconds::period>(diff).count();
            static decltype(diff) s_diff;
            static unsigned int s_count = 0;

            // Update simulation
            //g_sim.update(dt);
            //g_sim.update(1.f);
            //for (int i = 0; i < 10; ++i)
            if (!g_sim.update())
                exit(0);

            // Reset clock
            g_start = clock_type::now();

            // Add change
            s_diff += g_start - start;
            ++s_count;

            if (s_count == 100)
            {
                std::cout << "Avg Time: " << std::chrono::duration_cast<std::chrono::microseconds>(s_diff / s_count).count() << " us\n";

                s_diff = {};
                s_count = 0;
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(10));

            // Force redraw
            glutPostRedisplay();
        });

        glutKeyboardFunc([](unsigned char key, int x, int y) {
            switch (key)
            {
            case 'p': case 'P': g_paused = !g_paused; break;
            case 's': case 'S': if (g_paused) { g_sim.update(units::Time(0.01f)); glutPostRedisplay(); } break;
            case 'w': case 'W': g_sim.getRenderContext().setWireframe(!g_sim.getRenderContext().isWireframe()); break;
#if ENABLE_RENDER && ENABLE_PHYSICS_DEBUG
            case 'd': case 'D': g_sim.getSimulation()->setDrawPhysics(!g_sim.getSimulation()->isDrawPhysics()); break;
#endif
            }
        });

        // Init start time
        g_start = clock_type::now();

        glutMainLoop();
#else
        // Install signal handler
        signal(SIGINT, stop_simulation);

        // Start simulation
        g_sim.start();
#endif

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
