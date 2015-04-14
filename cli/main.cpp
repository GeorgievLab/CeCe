
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
#include "simulator/Simulation.hpp"
#include "parser/Parser.hpp"
#include "parser/SimulationFactory.hpp"
#include "modules/streamlines/Module.hpp"
#include "modules/diffusion/Module.hpp"
#include "modules/diffusion/Generator.hpp"
#include "modules/diffusion/GeneratorCell.hpp"
#include "modules/diffusion-streamlines/Module.hpp"
#include "modules/physics/Module.hpp"

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

module::streamlines::Module* g_streamlinesModule;
module::diffusion::Module* g_diffusionModule;

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

    try
    {
        // Create javascript world factory
        parser::SimulationFactory worldFactory;

        // Create world
        g_sim.setSimulation(worldFactory.fromFile(argv[1]));

        // Grid size
        auto grid_size = 300;

        // Create modules
        //g_streamlinesModule = g_sim.getSimulation()->createModule<module::streamlines::Module>(grid_size);
        //g_diffusionModule = g_sim.getSimulation()->createModule<module::diffusion::Module>(grid_size);
        auto ptr = g_sim.getSimulation()->createModule<module::diffusion_streamlines::Module>(grid_size);
        g_diffusionModule = &ptr->getDiffusion();
        g_diffusionModule->setCoefficients({{1.f, 0.005f, 0.01f, 10.f, 1.3f, 15.f, 1.f, 0.01f}});
        g_streamlinesModule = &ptr->getStreamlines();
        //g_diffusionModule->getDrawable().setInterpolate(false);
        //g_diffusionModule->getRenderGridObject().setRenderGrid(true);

        g_sim.getSimulation()->createModule<module::diffusion::Generator>(g_diffusionModule);
        g_sim.getSimulation()->createModule<module::diffusion::GeneratorCell>(g_diffusionModule);

        //g_sim.getSimulation()->createModule<module::physics::Module>();

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

        glutKeyboardFunc([](unsigned char key, int x, int y) {
            auto pos = g_streamlinesModule->getMainCellPosition();
            switch (key)
            {
            case 'a': case 'A': pos.getX() -= 0.5f; break;
            case 'd': case 'D': pos.getX() += 0.5f; break;
            case 'w': case 'W': pos.getY() += 0.5f; break;
            case 's': case 'S': pos.getY() -= 0.5f; break;
            case 'i': case 'I':
                g_diffusionModule->getDrawable().setInterpolate(!g_diffusionModule->getDrawable().isInterpolate());
                break;
            }

            g_streamlinesModule->setMainCellPosition(pos);
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
