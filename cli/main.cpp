
/* ************************************************************************ */

// C++
#include <iostream>
#include <string>
#include <exception>
#include <csignal>
#include <atomic>
#include <mutex>
#include <thread>
#ifdef ENABLE_RENDER
#include <chrono>
#endif

#ifdef ENABLE_RENDER
#include <GL/glut.h>
#endif

// Simulator
#include "simulator/Simulator.hpp"
#include "simulator/World.hpp"
#include "parser/Parser.hpp"
#include "parser/WorldFactory.hpp"
#include "simulator/StreamlinesModule.hpp"
#include "simulator/DiffusionModule.hpp"
#include "simulator/DiffusionStreamlinesModule.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

std::atomic_bool g_terminated{false};

/* ************************************************************************ */

// Simulator
simulator::Simulator g_sim;

/* ************************************************************************ */

/// Mutex for logging.
std::mutex g_log_mutex;

/* ************************************************************************ */

#ifdef ENABLE_RENDER
render::Context g_context;
#endif

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

#ifdef ENABLE_RENDER
    glutInit(&argc, argv);
    glutInitWindowSize(g_width, g_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutCreateWindow("Cell Simulator");

    g_context.init();
#endif

    try
    {
        // Create javascript world factory
        parser::WorldFactory worldFactory;

        // Create world
        g_sim.setWorld(worldFactory.fromFile(argv[1]));

        // Create modules
        //g_sim.createModule<simulator::StreamlinesModule>();
        //g_sim.createModule<simulator::DiffusionModule>();
        g_sim.createModule<simulator::DiffusionStreamlinesModule>();

#ifdef ENABLE_RENDER
        // Register callbacks:
        glutDisplayFunc([]() {
            g_context.frameBegin(g_width, g_height);
            g_sim.render(g_context);
            g_context.frameEnd();
            glutSwapBuffers();
        });

        glutReshapeFunc([](int width, int height) {
            g_width = width;
            g_height = height;
            auto size = g_sim.getWorld()->getSize();

            g_context.getCamera().setZoom(
                std::max(size.getWidth() / g_width, size.getHeight() / g_height)
            );
        });

        // Idle function: update scene
        glutIdleFunc([]() {
            auto start = clock_type::now();
            auto diff = start - g_start;
            float dt = std::chrono::duration<float, std::chrono::seconds::period>(diff).count();

            // Update simulation
            //g_sim.update(dt);
            //g_sim.update(1.f);
            //for (int i = 0; i < 10; ++i)
            g_sim.update(0.01f);

            // Reset clock
            g_start = clock_type::now();

            std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(g_start - start).count() << " us\n";

            //std::this_thread::sleep_for(std::chrono::milliseconds(10));

            // Force redraw
            glutPostRedisplay();
        });

        // Init start time
        g_start = clock_type::now();

        // Initialize objects for rendering
        g_sim.renderInit(g_context);

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
