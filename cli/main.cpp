/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
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

#if __APPLE__ && __MACH__
#include <libgen.h>
#endif

#if ENABLE_RENDER
#include <GLFW/glfw3.h>
#endif

#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE || CONFIG_CLI_ENABLE_IMAGE_CAPTURE
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

// Simulator
#include "core/Macro.hpp"
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/Exception.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/FilePath.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/PluginManager.hpp"

#if CONFIG_CLI_ENABLE_IMAGE_CAPTURE
#include "core/StringStream.hpp"
#endif

#if ENABLE_RENDER
#include "core/TriBool.hpp"
#include "render/Context.hpp"
#if ENABLE_PHYSICS_DEBUG
#include "render/PhysicsDebugger.hpp"
#endif
#endif

// Version
#include "version.hpp"

/* ************************************************************************ */

#if !CONFIG_CLI_VISUALIZE_WIDTH
#define CONFIG_CLI_VISUALIZE_WIDTH 800
#endif

#if !CONFIG_CLI_VISUALIZE_HEIGHT
#define CONFIG_CLI_VISUALIZE_HEIGHT 600
#endif

/* ************************************************************************ */

/**
 * @brief Application parameters.
 */
struct Arguments
{
    /// Path to simulation file.
    FilePath simulationFile;

    /// Simulation parameters.
    Parameters parameters;

#if ENABLE_RENDER
    // If simulation should be rendered.
    TriBool visualize = Indeterminate;

    /// Window width.
    unsigned int windowWidth = 0;

    /// Window height.
    unsigned int windowHeight = 0;

    // Visualization window in fullscreen mode.
    bool fullscreen = false;
#endif

#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
    /// Path to output video file.
    FilePath videoFileName;
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
[[noreturn]] static void printHelp(const char* name) noexcept
{
#if _WIN32
    char fname[_MAX_FNAME];
    char fext[_MAX_EXT];
    _splitpath(name, NULL, NULL, fname, fext);
    auto bname = String(fname) + "." + fext;
#else
    DynamicArray<char> localName(name, name + strlen(name));
    const char* bname = basename(localName.data());
#endif

    std::cout <<
        APP_NAME "\n"
        "\n"
        "  Cell-Cell interaction simulator.\n"
        "\n"
        "Version:\n"
        "    " VERSION "\n"
        "\n"
        "Usage:\n"
        "  " << bname << " "
            "[ --plugins "
            "| --param | -p "
#if ENABLE_RENDER
            "| --visualize "
            "| --novisualize "
            "| --fullscreen "
            "| --width <width> "
            "| --height <height> "
#endif
#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
            "| --capture <filename> "
#endif
            "] "
        "<simulation-file>\n"
        "\n"
        "    --plugins            Prints a list of available plugins.\n"
        "    --param <name> <value> Set simulation parameter.\n"
        "    -p <name> <value>    Set simulation parameter.\n"
#if ENABLE_RENDER
        "    --visualize          Enable visualization (default).\n"
        "    --fullscreen         Visualization window in fullscreen mode.\n"
        "    --novisualize        Disable visualization.\n"
        "    --width <width>      Visualization window width "
            "(default " XSTR(CONFIG_CLI_VISUALIZE_WIDTH) "; fullscreen by monitor).\n"
        "    --height <height>    Visualization window height "
            "(default " XSTR(CONFIG_CLI_VISUALIZE_HEIGHT) "; fullscreen by monitor).\n"
#endif
#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
        "    --capture <filename> Capture video of the simulation.\n"
#endif
        "    <simulation-file>    Path to simulation file.\n"
    << std::endl;

    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Prints a list of available plugins.
 */
[[noreturn]] static void printPlugins() noexcept
{
    std::cout <<
        "Plugins:\n";

    for (auto name : simulator::PluginManager::s().getNames())
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
 * @return Arguments.
 */
Arguments parseArguments(int argc, char** argv)
{
    Arguments args;

    for (int i = 1; i < argc; ++i)
    {
        StringView arg(argv[i]);

        // Switch
        if (arg[0] == '-')
        {
            if (arg == "-p" || arg == "--param")
            {
                // Parameter
                if (i + 2 >= argc)
                    throw InvalidArgumentException("Missing parameter value");

                // Store parameter
                args.parameters[argv[i + 1]] = units::parse(argv[i + 2]);
                i += 2;
            }
#if ENABLE_RENDER
            else if (arg == "--visualize")
            {
                args.visualize = true;
            }
            else if (arg == "--novisualize")
            {
                args.visualize = false;
            }
            else if (arg == "--fullscreen")
            {
                args.fullscreen = true;
            }
            else if (arg == "--width")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("missing width value");

                args.windowWidth = atoi(argv[i + 1]);
                ++i;

                if (!args.windowWidth)
                    throw InvalidArgumentException("invalid width value");
            }
            else if (arg == "--height")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("missing height value");

                args.windowHeight = atoi(argv[i + 1]);
                ++i;

                if (!args.windowHeight)
                    throw InvalidArgumentException("invalid height value");
            }
            else
#endif
#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
            if (arg == "--capture")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("missing filename value");

                args.videoFileName = argv[i + 1];
                ++i;
            }
            else
#endif
            if (arg == "--plugins")
                printPlugins();
            else if (arg == "--help")
                printHelp(argv[0]);
        }
        else if (args.simulationFile.empty())
        {
            args.simulationFile = FilePath(arg.getData());
        }
        else
        {
            Log::warning("Unknown argument: ", arg.getData());
        }
    }

    if (args.simulationFile.empty())
        throw InvalidArgumentException("missing simulation file");

    return args;
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
     * @param args Simulation arguments.
     */
    explicit Simulator(const Arguments& args)
    {
        m_simulationFile = args.simulationFile;

        // Create simulation
        m_simulator.setSimulation(simulator::PluginManager::s().createSimulation(m_simulationFile));

        // Set parameters
        m_simulator.getSimulation()->setParameters(std::move(args.parameters));

#if ENABLE_RENDER
        const auto simViz = m_simulator.getSimulation()->getVisualize();

        // Decide if simulation should be visualized
        if (args.visualize)
            m_visualize = true;
        else if (!args.visualize)
            m_visualize = false;
        else
            m_visualize = simViz || indeterminate(simViz);

        initVisualization(args);
#endif
    }


    /**
     * @brief Destructor.
     */
    ~Simulator()
    {
        // Delete simulation
        m_simulator.setSimulation(nullptr);
#if ENABLE_RENDER
        // Delete all released object before visualization cleanup
        m_simulator.getRenderContext().deleteReleasedObjects();
        cleanupVisualization();
#endif
    }

// Public Accessors
public:


    /**
     * @brief Returns simulation.
     *
     * @return
     */
    simulator::Simulation* getSimulation() const noexcept
    {
        return m_simulator.getSimulation();
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

            // VSync
            glfwSwapInterval(1);

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

#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
        // Store image
        if (m_videoWriter.isOpened())
        {
            // Store image
            m_videoWriter << getFrame(false);
        }
#endif
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
        //ptr->setOptimalZoom();

        if (ptr->m_paused)
        {
            ptr->draw();

            // Swap buffers
            glfwSwapBuffers(win);
        }
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
        constexpr float MOVE_COEFF = 5;
        constexpr float ZOOM_COEFF = 1.1;

        auto* ptr = reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win));

        if (action != GLFW_PRESS && action != GLFW_REPEAT)
            return;

        switch (key)
        {
        default:
            // Nothing
            break;

        case GLFW_KEY_I:
        {
            auto& camera = ptr->m_simulator.getRenderContext().getCamera();
            camera.setZoom(camera.getZoom() * 1.0 / ZOOM_COEFF);
            break;
        }

        case GLFW_KEY_O:
        {
            auto& camera = ptr->m_simulator.getRenderContext().getCamera();
            camera.setZoom(camera.getZoom() * ZOOM_COEFF);
            break;
        }

        case GLFW_KEY_RIGHT:
        {
            auto& camera = ptr->m_simulator.getRenderContext().getCamera();
            camera.setPosition(camera.getPosition() +
                MOVE_COEFF * PositionVector{units::Length(-1), Zero}
            );
            break;
        }

        case GLFW_KEY_LEFT:
        {
            auto& camera = ptr->m_simulator.getRenderContext().getCamera();
            camera.setPosition(camera.getPosition() +
                MOVE_COEFF * PositionVector{units::Length(1), Zero}
            );
            break;
        }

        case GLFW_KEY_UP:
        {
            auto& camera = ptr->m_simulator.getRenderContext().getCamera();
            camera.setPosition(camera.getPosition() +
                MOVE_COEFF * PositionVector{Zero, units::Length(-1)}
            );
            break;
        }

        case GLFW_KEY_DOWN:
        {
            auto& camera = ptr->m_simulator.getRenderContext().getCamera();
            camera.setPosition(camera.getPosition() +
                MOVE_COEFF * PositionVector{Zero, units::Length(1)}
            );
            break;
        }

        case GLFW_KEY_R:
            // Reset camera
            ptr->setOptimalZoom();
            break;

        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(win, true);
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

#if CONFIG_CLI_ENABLE_IMAGE_CAPTURE
        case GLFW_KEY_C:
        {
            OutStringStream oss;

// Use working directory on Mac OS X
#if __APPLE__ && __MACH__
            // Get working directory
            const char* dir = getenv("WORKING_DIR");

            // Add directory
            if (dir)
                oss << dir << "/";
#endif

            // Add filename
            oss << ptr->m_simulationFile.stem().string() << "_" << ptr->getSimulation()->getIteration() << ".png";

            const String filename = oss.str();

            cv::imwrite(filename, ptr->getFrame(true));
            Log::info("Image captured: ", filename);
            break;
        }
#endif

#if ENABLE_PHYSICS_DEBUG
        case GLFW_KEY_D:
            ptr->swapPhysicsDebug();
            ptr->draw();

            // Swap buffers
            glfwSwapBuffers(win);
            break;
#endif

#if ENABLE_RENDER
        case GLFW_KEY_V:
            auto streamlines = ptr->getSimulation()->getModule("streamlines");

            if (streamlines)
            {
                if (streamlines->getDrawFlags() & 0x1)
                    streamlines->setDrawFlags(streamlines->getDrawFlags() & ~0x1);
                else
                    streamlines->setDrawFlags(streamlines->getDrawFlags() | 0x1);

                // Swap buffers
                glfwSwapBuffers(win);
            }

            break;
#endif
        }
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Mouse scroll callback.
     *
     * @param win
     * @param xoffset
     * @param yoffset
     */
    static void mouseScrollCallback(GLFWwindow* win, double xoffset, double yoffset) noexcept
    {
        constexpr float ZOOM_COEFF = 1.1;

        auto* ptr = reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win));
        auto& camera = ptr->m_simulator.getRenderContext().getCamera();

        if (yoffset < 0)
        {
            camera.setZoom(camera.getZoom() * ZOOM_COEFF);
        }
        else if (yoffset > 0)
        {
            camera.setZoom(camera.getZoom() * 1.0 / ZOOM_COEFF);
        }

        if (ptr->m_paused)
        {
            ptr->draw();

            // Swap buffers
            glfwSwapBuffers(win);
        }
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Mouse button callback.
     *
     * @param win
     * @param button
     * @param action
     * @param mods
     */
    static void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) noexcept
    {
        auto* ptr = reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win));

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                glfwGetCursorPos(win, &ptr->m_movePos.x(), &ptr->m_movePos.y());
            }
            else if (action == GLFW_RELEASE)
            {
                ptr->m_movePos = Zero;

                // Get position
                double x;
                double y;
                glfwGetCursorPos(win, &x, &y);

                // Raycast
                ptr->raycast(x, y);
            }
        }
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Mouse position callback.
     *
     * @param win
     * @param xpos
     * @param ypos
     */
    static void mousePosCallback(GLFWwindow* win, double xpos, double ypos) noexcept
    {
        if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
            return;

        auto* ptr = reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win));
        auto& camera = ptr->m_simulator.getRenderContext().getCamera();

        auto pos = camera.getPosition();

        // Change vector
        auto change = BasicVector<double, 2>(xpos, ypos) - ptr->m_movePos;

        pos += camera.getZoom() * PositionVector{
            units::Length(change.getX()),
            units::Length(-change.getY())
        };

        camera.setPosition(pos);

        ptr->m_movePos = BasicVector<double, 2>(xpos, ypos);

        if (ptr->m_paused)
        {
            ptr->draw();

            // Swap buffers
            glfwSwapBuffers(win);
        }
    }
#endif


// Private Operations
private:


#if ENABLE_RENDER
    /**
     * @brief Init visualization.
     *
     * @param args Arguments.
     */
    void initVisualization(const Arguments& args)
    {
        if (!m_visualize)
            return;

        // Set callback
        glfwSetErrorCallback(visualizationErrorCallback);

        // Initialize the library
        if (!glfwInit())
            throw RuntimeException("Unable to initialize GLFW");

        // Get primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        // Monitor video mode
        const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

        // Fullscreen mode
        if (args.fullscreen)
        {
            m_windowWidth  = args.windowWidth  ? args.windowWidth  : vidmode->width;
            m_windowHeight = args.windowHeight ? args.windowHeight : vidmode->height;

            // Borderless fullscreen mode
            if (!args.windowWidth && !args.windowHeight)
            {
                glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
            }
        }
        else
        {
            m_windowWidth  = args.windowWidth
                ? args.windowWidth
                : CONFIG_CLI_VISUALIZE_WIDTH;
            m_windowHeight = args.windowHeight
                ? args.windowHeight
                : CONFIG_CLI_VISUALIZE_HEIGHT;
        }

#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
        if (!args.videoFileName.empty())
        {
            String filename;

// Use working directory on Mac OS X
#if __APPLE__ && __MACH__
            // Get working directory
            const char* dir = getenv("WORKING_DIR");

            // Add directory
            if (dir)
                filename += String(dir) + "/";
#endif
            filename += args.videoFileName.string();

            Log::info("Video output '", filename, "'");

            // Open video writer
            m_videoWriter.open(
                filename,
#if _WIN32
                CV_FOURCC('M', 'J', 'P', 'G'),
#elif __APPLE__ && __MACH__
                CV_FOURCC('M', 'P', '4', 'V'),
#else
                CV_FOURCC('M', 'P', 'E', 'G'),
#endif
                60,
                cv::Size(m_windowWidth, m_windowHeight)
            );

            // Disable window resizing
            if (m_videoWriter.isOpened())
                glfwWindowHint(GLFW_RESIZABLE, false);
            else
                Log::warning("Unable to capture video");
        }
#endif

        // Create a windowed mode window and its OpenGL context
        m_window = glfwCreateWindow(
            m_windowWidth,
            m_windowHeight,
            APP_NAME " simulator",
            args.fullscreen ? monitor : nullptr,
            nullptr
        );

        if (!m_window)
            throw RuntimeException("Unable to create rendering window");

        if (!args.fullscreen)
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

        // Set callbacks
        glfwSetWindowSizeCallback(m_window, &Simulator::windowResizeCallback);
        glfwSetKeyCallback(m_window, &Simulator::keyboardCallback);
        glfwSetScrollCallback(m_window, &Simulator::mouseScrollCallback);
        glfwSetMouseButtonCallback(m_window, &Simulator::mouseButtonCallback);
        glfwSetCursorPosCallback(m_window, &Simulator::mousePosCallback);
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Visualization error callback.
     *
     * @param error
     * @param description
     */
    static void visualizationErrorCallback(int error, const char* description) noexcept
    {
        Log::warning(description);
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Cleanup visualization.
     */
    void cleanupVisualization() noexcept
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
        // Get simulation
        auto simulation = m_simulator.getSimulation();

#if ENABLE_PHYSICS_DEBUG

        m_physicsDebugger.SetFlags(
            render::PhysicsDebugger::e_shapeBit |
            render::PhysicsDebugger::e_centerOfMassBit |
            render::PhysicsDebugger::e_jointBit
        );
        simulation->getWorld().SetDebugDraw(&m_physicsDebugger);
#endif

        // Initialize simulator
        m_simulator.drawInit(simulation->getBackgroundColor());

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
        auto& camera = m_simulator.getRenderContext().getCamera();
        camera.setPosition(Zero);
        camera.setZoom(
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


    /**
     * @brief Raycast scene.
     *
     * @param mouseX
     * @param mouseY
     *
     * @see http://antongerdelan.net/opengl/raycasting.html
     */
    void raycast(float mouseX, float mouseY) noexcept
    {
        const auto& simulation = m_simulator.getSimulation();
        const auto& camera = m_simulator.getRenderContext().getCamera();
        const auto size = simulation->getWorldSize();
        const auto zoom = std::max(size.getWidth() / m_windowWidth, size.getHeight() / m_windowHeight).value();

        // Normalized
        const BasicVector<float, 2> posNorm(
            (2.0f * mouseX) / m_windowWidth - 1.0f,
            1.0f - (2.0f * mouseY) / m_windowHeight
        );

        // To world coordinates
        PositionVector posWorld = 0.5 * posNorm * size;

        // Zoom
        posWorld *= (camera.getZoom() / zoom);

        // Translate
        posWorld -= camera.getPosition();

        // Find object
        auto object = simulation->query(posWorld);

        if (object)
            Log::info("Object: id = ", object->getId(), ", className = ", object->getClassName());
    }


#if CONFIG_CLI_ENABLE_IMAGE_CAPTURE || CONFIG_CLI_ENABLE_VIDEO_CAPTURE
    /**
     * @brief Get current OpenGL frame.
     *
     * @param alpha If alfa should be stored.
     *
     * @return Current frame.
     */
    cv::Mat getFrame(bool alpha) const noexcept
    {
        const auto& context = m_simulator.getRenderContext();
        const auto size = context.getSize();

        // Get image data
        cv::Mat img(size.getHeight(), size.getWidth(), alpha ? CV_8UC4 : CV_8UC3);
        context.getData(img.data, alpha, true);
        cv::flip(img, img, 0);
        return img;
    }
#endif

// Private Data Members
private:

#if ENABLE_PHYSICS_DEBUG
    // Physics engine debug draw.
    render::PhysicsDebugger m_physicsDebugger;
#endif

    /// Path to simulation file.
    FilePath m_simulationFile;

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

    /// Last position for move cursor.
    BasicVector<double, 2> m_movePos;
#endif

#if CONFIG_CLI_ENABLE_VIDEO_CAPTURE
    /// Video writer.
    cv::VideoWriter m_videoWriter;
#endif
};

/* ************************************************************************ */

/**
 * @brief Returns plugins directory.
 *
 * @param path Executable path.
 *
 * @return
 */
String getPluginsDirectory(const char* path) noexcept
{
#if _WIN32
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath(path, drive, dir, NULL, NULL);
    return String(drive) + dir + DIR_PLUGINS;
#else
    // Absolute path to plugins directory on Linux
    return DIR_PLUGINS;
#endif
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
    // Install signal handler
    signal(SIGTERM, terminate_simulation);
    signal(SIGINT, terminate_simulation);

#if !(__APPLE__ && __MACH__)
    // Register plugins directory
    simulator::PluginManager::s().addDirectory(getPluginsDirectory(argv[0]));
#endif
    simulator::PluginManager::s().init();

#if ENABLE_MEASUREMENT
    std::ofstream time_file("time.csv");
    setMeasureTimeOutput(&time_file);
#endif

    try
    {
        // Create simulator and start it
        Simulator(parseArguments(argc, argv)).start();
    }
    catch (const Exception& e)
    {
        Log::error(e.what());
        return 1;
    }
    catch (...)
    {
        Log::error("Unknown exception");
        return 1;
    }

    // In case of termination, return special code
    return g_terminated ? 2 : 0;
}

/* ************************************************************************ */
