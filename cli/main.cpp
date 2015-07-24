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
#if _WIN32
// _splitpath
#include <stdlib.h>
#endif

#if ENABLE_RENDER
#include <GLFW/glfw3.h>
#endif

// Simulator
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/Exception.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/FilePath.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/PluginManager.hpp"
#include "parser-xml/SimulationFactory.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"
#if ENABLE_PHYSICS_DEBUG
#include "render/PhysicsDebugger.hpp"
#endif
#endif

/* ************************************************************************ */

/**
 * @brief Application parameters.
 */
struct Parameters
{
    /// Path to simulation file.
    FilePath simulationFile;

#if ENABLE_RENDER
    // If simulation should be rendered.
    bool visualize = true;

    /// Window width.
    unsigned int windowWidth = 0;

    /// Window height.
    unsigned int windowHeight = 0;

    // Vizualization window in fullscreen mode.
    bool fullscreen = false;
#endif
};

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
 * @brief Prints help message.
 *
 * @param name Application name.
 */
[[noreturn]] static void help(const char* name) noexcept
{
#if _WIN32
    char fname[_MAX_FNAME];
    char fext[_MAX_EXT];
    _splitpath(name, NULL, NULL, fname, fext);
    auto bname = String(fname) + "." + fext;
#else
    const char* bname = basename(name);
#endif

    std::cout <<
        "Simulator\n"
        "\n"
        "  TODO: description.\n"
        "\n"
        "Usage:\n"
        "  " << bname << " "
            "[ --plugins "
#if ENABLE_RENDER
            "| --vizualize "
            "| --novizualize "
            "| --fullscreen "
            "| --width <width> "
            "| --height <height> "
#endif
            "] "
        "<simulation-file>\n"
        "\n"
        "    --plugins          Prints a list of available plugins.\n"
#if ENABLE_RENDER
        "    --vizualize        Enable vizualization (default).\n"
        "    --fullscreen       Vizualization window in fullscreen mode.\n"
        "    --novizualize      Disable vizualization.\n"
        "    --width <width>    Vizualization window width (default 800; fullscreen by monitor).\n"
        "    --height <height>  Vizualization window height (default 600; fullscreen by monitor).\n"
#endif
        "    <simulation-file>   Path to simulation file.\n"
    << std::endl;

    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Prints a list of available plugins.
 */
[[noreturn]] static void plugins() noexcept
{
    std::cout <<
        "Plugins:\n";

    for (auto name : simulator::PluginManager::getNames())
    {
        std::cout << "    " << name << "\n";
    }

    std::cout << std::endl;

    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Parse arguments.
 *
 * @param argc
 * @param argv
 *
 * @return Parameters.
 */
Parameters parseArguments(int argc, char** argv)
{
    Parameters params;

    for (int i = 1; i < argc; ++i)
    {
        StringView arg(argv[i]);

        // Switch
        if (arg[0] == '-')
        {
#if ENABLE_RENDER
            if (arg == "--vizualize")
            {
                params.visualize = true;
            }
            else if (arg == "--novizualize")
            {
                params.visualize = false;
            }
            else if (arg == "--fullscreen")
            {
                params.fullscreen = true;
            }
            else if (arg == "--width")
            {
                if (i + 1 < argc)
                    throw InvalidArgumentException("missing width value");

                params.windowWidth = atoi(argv[i + 1]);

                if (!params.windowWidth)
                    throw InvalidArgumentException("invalid width value");
            }
            else if (arg == "--height")
            {
                if (i + 1 < argc)
                    throw InvalidArgumentException("missing height value");

                params.windowHeight = atoi(argv[i + 1]);

                if (!params.windowHeight)
                    throw InvalidArgumentException("invalid height value");
            }
            else
#endif
            if (arg == "--plugins")
                plugins();
            else if (arg == "--help")
                help(argv[0]);
        }
        else if (params.simulationFile.empty())
        {
            params.simulationFile = FilePath(arg.getData());
        }
        else
        {
            Log::warning("Unknown argument: ", arg.getData());
        }
    }

    if (params.simulationFile.empty())
        throw InvalidArgumentException("missing simulation file");

    return params;
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
     * @param params Simulation parameters.
     */
    explicit Simulator(const Parameters& params)
    {
        // Create simulation factory
        // TODO: loaders
        parser::xml::SimulationFactory simFactory;

        // Create simulation
        m_simulator.setSimulation(simFactory.fromFile(params.simulationFile));

#if ENABLE_RENDER
        initVizualization(params);
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

            // Initialize scene
            initScene();

            // Loop until the user closes the window
            while (!g_terminated && !glfwWindowShouldClose(m_window))
            {
                if (!m_paused)
                {
                    // Update scene
                    if (!update())
                        break;

                    // Draw scene
                    draw();

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
            while (!g_terminated && update())
                continue;
#if ENABLE_RENDER
        }
#endif
    }


    /**
     * @brief Update simulation.
     *
     * @return Update result.
     */
    bool update()
    {
        return m_simulator.update();
    }


#if ENABLE_RENDER
    /**
     * @brief Draw scene.
     */
    void draw()
    {
        m_simulator.draw(m_windowWidth, m_windowHeight);
    }
#endif


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

        // Update zoom
        ptr->setOptimalZoom();
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

        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(win, GL_TRUE);
            break;

        case GLFW_KEY_P:
            ptr->m_paused = !ptr->m_paused;
            break;

        case GLFW_KEY_S:
            if (ptr->m_paused)
            {
                ptr->update();
                ptr->draw();

                // Swap buffers
                glfwSwapBuffers(win);
            }
            break;

#if ENABLE_PHYSICS_DEBUG
        case GLFW_KEY_D:
            ptr->swapPhysicsDebug();
            break;
#endif
        }
    }
#endif


// Private Operations
private:


#if ENABLE_RENDER
    /**
     * @brief Init vizualization.
     *
     * @param params Parameters.
     */
    void initVizualization(const Parameters& params)
    {
        m_visualize = params.visualize;

        if (!m_visualize)
            return;

        // Initialize the library
        if (!glfwInit())
            throw RuntimeException("Unable to initialize GLFW");

        // Get primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        // Monitor video mode
        const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

        // Fullscreen mode
        if (params.fullscreen)
        {
            m_windowWidth  = params.windowWidth  ? params.windowWidth  : vidmode->width;
            m_windowHeight = params.windowHeight ? params.windowHeight : vidmode->height;

            // Borderless fullscreen mode
            if (!params.windowWidth && !params.windowHeight)
            {
                glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
            }
        }
        else
        {
            m_windowWidth  = params.windowWidth  ? params.windowWidth  : 800;
            m_windowHeight = params.windowHeight ? params.windowHeight : 600;
        }

        // Create a windowed mode window and its OpenGL context
        m_window = glfwCreateWindow(
            m_windowWidth,
            m_windowHeight,
            "Simulator",
            params.fullscreen ? monitor : nullptr,
            nullptr
        );

        if (!m_window)
            throw RuntimeException("Unable to create rendering window");

        if (!params.fullscreen)
        {
            // Get monitor position
            int xpos;
            int ypos;
            glfwGetMonitorPos(monitor, &xpos, &ypos);

            // Center window
            glfwSetWindowPos(
                m_window,
                xpos + (vidmode->width - m_windowWidth) / 2,
                ypos + (vidmode->height - m_windowHeight) / 2
            );
        }

        // Store this pointer
        glfwSetWindowUserPointer(m_window, this);

        //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_STENCIL);
        glfwWindowHint(GLFW_SAMPLES, 8);

        // VSync
        glfwSwapInterval(1);

        // Set callbacks
        glfwSetWindowSizeCallback(m_window, &Simulator::windowResizeCallback);
        glfwSetKeyCallback(m_window, &Simulator::keyboardCallback);
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


#if ENABLE_RENDER
    /**
     * @brief Initialize rendering scene.
     */
    void initScene()
    {
#if ENABLE_PHYSICS_DEBUG
        // Get simulation
        auto simulation = m_simulator.getSimulation();

        m_physicsDebugger.SetFlags(
            render::PhysicsDebugger::e_shapeBit |
            render::PhysicsDebugger::e_centerOfMassBit
        );
        simulation->getWorld().SetDebugDraw(&m_physicsDebugger);
#endif

        // Initialize simulator
        m_simulator.drawInit();

        // Initial zoom
        setOptimalZoom();
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Calculate zoom to render scene in whole window.
     */
    void setOptimalZoom()
    {
        auto size = m_simulator.getSimulation()->getWorldSize();
        m_simulator.getRenderContext().getCamera().setZoom(
            std::max(size.getWidth() / m_windowWidth, size.getHeight() / m_windowHeight).value()
        );
    }
#endif


#if ENABLE_RENDER && ENABLE_PHYSICS_DEBUG
    /**
     * @brief Swap physics debug rendering.
     */
    void swapPhysicsDebug() noexcept
    {
        m_simulator.getSimulation()->setDrawPhysics(!m_simulator.getSimulation()->isDrawPhysics());
    }
#endif


// Private Data Members
private:

    // Simulator
    simulator::Simulator m_simulator;

#if ENABLE_RENDER
    // If simulation should be rendered.
    bool m_visualize;

    /// Window width.
    unsigned int m_windowWidth;

    /// Window height.
    unsigned int m_windowHeight;

    /// Render window.
    GLFWwindow* m_window;

    /// If simulation is paused.
    bool m_paused = false;

#if ENABLE_PHYSICS_DEBUG
    // Physics engine debug draw.
    render::PhysicsDebugger m_physicsDebugger;
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
        // Parse arguments
        auto params = parseArguments(argc, argv);

        // Create simulator and start it
        Simulator(params).start();
    }
    catch (const Exception& e)
    {
        Log::error(e.what());
        help(argv[0]);
        return 1;
    }
}

/* ************************************************************************ */
