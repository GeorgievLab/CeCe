
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
#include "simulator/Library.hpp"
#include "simulator/Module.hpp"
#include "parser/Parser.hpp"
#include "parser/SimulationFactory.hpp"

#include "modules/streamlines/Module.hpp"
#include "modules/diffusion/Module.hpp"
#include "modules/diffusion/Generator.hpp"
#include "modules/diffusion/GeneratorCell.hpp"
#include "modules/diffusion-streamlines/Module.hpp"
#include "modules/cell/Generator.hpp"
#include "modules/cell/Yeast.hpp"
#include "modules/cell/Cell.hpp"

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

module::streamlines::Module* g_streamlinesModule;
module::diffusion::Module* g_diffusionModule;
module::cell::Cell* g_mainCell;

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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_STENCIL);
    glutCreateWindow("Cell Simulator");

    g_context.init();
#endif

    std::ofstream time_file("time.csv");

    try
    {
        setMeasureTimeOutput(&time_file);

        // Create javascript world factory
        parser::SimulationFactory simFactory;

        // Create world
        g_sim.setSimulation(simFactory.fromFile(g_sim, argv[1]));

        // Get simulation
        auto simulation = g_sim.getSimulation();

#if ENABLE_PHYSICS_DEBUG
        DebugDraw debugDraw;
        debugDraw.SetFlags(DebugDraw::e_shapeBit | DebugDraw::e_centerOfMassBit);
        simulation->getWorld().SetDebugDraw(&debugDraw);
#endif

        // Load modules
        //simulation->useModule("diffusion-streamlines");
        //simulation->useModule("streamlines");
        //simulation->useModule("diffusion.generator");
        //simulation->useModule("diffusion.generator-cell");
        simulation->useModule("cell.generator");

        // Create main cell
        g_mainCell = simulation->createObject<module::cell::Cell>(simulator::Object::Type::Static);
        g_mainCell->setVolume(units::um3(1540));

        // Create modules
        g_diffusionModule = simulation->getModule<module::diffusion::Module>("diffusion");

        if (g_diffusionModule)
        {
#ifdef __AVX__
            g_diffusionModule->setCoefficients({{1.f, 0.005f, 0.001f, 10.f, 1.3f, 1.5f, 0.1f, 0.01f}});
#else
            g_diffusionModule->setCoefficients({{1.f, 0.005f, 0.001f, 10.f}});
#endif
        }

        g_streamlinesModule = simulation->getModule<module::streamlines::Module>("streamlines");
        if (g_streamlinesModule)
        {
            g_streamlinesModule->setFlowSpeed(10);
            //g_streamlinesModule->getRenderObject().setRenderVelocity(true);
        }

        //g_sim.getSimulation()->createModule<module::diffusion::Generator>(g_diffusionModule);
        //g_sim.getSimulation()->createModule<module::diffusion::GeneratorCell>(g_diffusionModule);
        //g_sim.getSimulation()->createModule<module::cell::Generator>();

#ifdef ENABLE_RENDER
        // Register callbacks:
        glutDisplayFunc([]() {
            g_context.frameBegin(g_width, g_height);
            g_sim.draw(g_context);
            g_context.frameEnd();
            glutSwapBuffers();
        });

        glutReshapeFunc([](int width, int height) {
            g_width = width;
            g_height = height;
            auto size = g_sim.getSimulation()->getWorldSize();

            g_context.getCamera().setZoom(
                std::max(size.getWidth() / g_width, size.getHeight() / g_height)
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
            g_sim.update(0.01f);

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

        glutSpecialFunc([](int key, int x, int y) {
            if (g_mainCell)
            {
                auto pos = g_mainCell->getPosition();
                switch (key)
                {
                case GLUT_KEY_LEFT: pos.getX() -= 0.5f; break;
                case GLUT_KEY_RIGHT: pos.getX() += 0.5f; break;
                case GLUT_KEY_UP: pos.getY() += 0.5f; break;
                case GLUT_KEY_DOWN: pos.getY() -= 0.5f; break;
                }

                g_mainCell->setPosition(pos);
                g_streamlinesModule->markUpdate();
            }
        });

        glutKeyboardFunc([](unsigned char key, int x, int y) {
            if (g_diffusionModule)
            {
                switch (key)
                {
                case 'i': case 'I':
                    g_diffusionModule->getDrawable().setInterpolate(!g_diffusionModule->getDrawable().isInterpolate());
                    break;
                }
            }

            switch (key)
            {
            case 'p': case 'P': g_paused = !g_paused; break;
            case 's': case 'S': if (g_paused) { g_sim.update(0.01f); glutPostRedisplay(); } break;
            }
        });

        // Init start time
        g_start = clock_type::now();

        // Initialize objects for rendering
        g_sim.drawInit(g_context);

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
