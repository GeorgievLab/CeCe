/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// C++
#include <iostream>
#include <csignal>
#include <atomic>
#include <fstream>

#if ENABLE_RENDER
#include <GLFW/glfw3.h>
#endif

// Simulator
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/Exception.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/PluginManager.hpp"
#include "parser-xml/SimulationFactory.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"

#if ENABLE_PHYSICS_DEBUG
#include "Render.hpp"
#endif

#endif

/* ************************************************************************ */

/// Global termination flag
std::atomic_bool g_terminated{false};

/* ************************************************************************ */

/**
 * @brief Terminate simulation.
 *
 * @param param
 */
void terminate_simulation(int param)
{
    std::cerr << "Terminating simulation" << std::endl;
    g_terminated = true;
}

/* ************************************************************************ */

/**
 * @brief Simulator.
 */
class Simulator
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param argc
     * @param argv
     */
    Simulator(int argc, char** argv)
    {
        parseArguments(argc, argv);

#if ENABLE_RENDER
        initVizualization();
#endif
    }


    /**
     * @brief Destructor.
     */
    ~Simulator()
    {
#if ENABLE_RENDER
        cleanupVizualization();
#endif
    }


// Public Operations
public:


    /**
     * @brief Start simulation.
     */
    void start()
    {
#if ENABLE_RENDER
        if (m_visualize)
        {
            // Make the window's context current
            glfwMakeContextCurrent(m_window);

            // Loop until the user closes the window
            while (!g_terminated && !glfwWindowShouldClose(m_window))
            {
                if (!m_paused)
                {
                    // Update scene
                    m_simulator.update();

                    // Draw scene
                    m_simulator.draw(m_windowWidth, m_windowHeight);

                    // Swap buffers
                    glfwSwapBuffers(m_window);
                }

                /// Poll for and process events
                glfwPollEvents();
            }
        }
        else
        {
#endif
            // Update simulation
            while (!g_terminated && m_simulator.update())
                continue;
#if ENABLE_RENDER
        }
#endif
    }


#if ENABLE_RENDER
    /**
     * @brief Window resize callback.
     *
     * @param win
     * @param w
     * @param h
     */
    static void windowResizeCallback(GLFWwindow* win, int w, int h) noexcept
    {
        auto* ptr = reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win));
        ptr->m_windowWidth = w;
        ptr->m_windowHeight = h;

        auto size = ptr->m_simulator.getSimulation()->getWorldSize();
        ptr->m_simulator.getRenderContext().getCamera().setZoom(
            std::max(size.getWidth() / w, size.getHeight() / h).value()
        );
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Keyboard callback function.
     *
     * @param win
     * @param key
     * @param code
     * @param action
     * @param mods
     */
    static void keyboardCallback(GLFWwindow* win, int key, int code, int action, int mods) noexcept
    {
        auto* ptr = reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win));

        if (action != GLFW_PRESS)
            return;

        switch (key)
        {
        default:
            // Nothing
            break;

        case GLFW_KEY_P:
            ptr->m_paused = !ptr->m_paused;
            break;

        case GLFW_KEY_S:
            if (ptr->m_paused)
            {
                ptr->m_simulator.update();
                // TODO: redraw
            }
            break;

#if ENABLE_PHYSICS_DEBUG
        case GLFW_KEY_D:
            g_sim.getSimulation()->setDrawPhysics(!g_sim.getSimulation()->isDrawPhysics());
            break;
#endif
        }
    }
#endif


// Private Operations
private:


    /**
     * @brief Parse arguments.
     *
     * @param argc
     * @param argv
     */
    void parseArguments(int argc, char** argv)
    {
        if (argc < 2)
        {
            throw InvalidArgumentException("not enough arguments: <desc.xml>");
        }

        // Create simulation factory
        parser::xml::SimulationFactory simFactory;

        // Create simulation
        m_simulator.setSimulation(simFactory.fromFile(argv[1]));
    }


#if ENABLE_RENDER
    /**
     * @brief Init vizualization.
     */
    void initVizualization()
    {
        if (!m_visualize)
            return;

        // Initialize the library
        if (!glfwInit())
            throw RuntimeException("Unable to initialize GLFW");

        // Create a windowed mode window and its OpenGL context
        m_window = glfwCreateWindow(
            m_windowWidth,
            m_windowHeight,
            "Simulator", nullptr, nullptr
        );

        if (!m_window)
            throw RuntimeException("Unable to create rendering window");

        // Store this pointer
        glfwSetWindowUserPointer(m_window, this);

        //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_STENCIL);

        // VSync
        glfwSwapInterval(1);

        // Set callbacks
        glfwSetWindowSizeCallback(m_window, &Simulator::windowResizeCallback);
        glfwSetKeyCallback(m_window, &Simulator::keyboardCallback);

#if ENABLE_PHYSICS_DEBUG
        // Get simulation
        auto simulation = m_simulator.getSimulation();

        m_debugDraw.SetFlags(DebugDraw::e_shapeBit | DebugDraw::e_centerOfMassBit);
        simulation->getWorld().SetDebugDraw(&m_debugDraw);
#endif

        // Initialize simulator
        m_simulator.drawInit();
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Cleanup vizualization.
     */
    void cleanupVizualization() noexcept
    {
        if (!m_visualize)
            return;

        if (m_window)
            glfwDestroyWindow(m_window);

        glfwTerminate();
    }
#endif


// Private Data Members
private:

    // Simulator
    simulator::Simulator m_simulator;

#if ENABLE_RENDER
    // If simulation should be rendered.
    bool m_visualize = false;

    /// Window width.
    unsigned int m_windowWidth = 800;

    /// Window height.
    unsigned int m_windowHeight = 600;

    /// Render window.
    GLFWwindow* m_window;

    /// If simulation is paused.
    bool m_paused = false;

#if ENABLE_PHYSICS_DEBUG
    // Physics engine debug draw.
    DebugDraw m_debugDraw;
#endif

#endif
};

/* ************************************************************************ */

/**
 * @brief Entry function.
 *
 * @param argc
 * @param argv
 */
int main(int argc, char** argv)
{
    // Install signal handler
    signal(SIGTERM, terminate_simulation);
    signal(SIGINT, terminate_simulation);

    // Initialize plugins
    simulator::PluginManager::rescanDirectories();

    std::ofstream time_file("time.csv");
    setMeasureTimeOutput(&time_file);

    try
    {
        // Create simulator
        Simulator(argc, argv).start();
    }
    catch (const Exception& e)
    {
        Log::error(e.what());
        return 1;
    }
}

/* ************************************************************************ */
