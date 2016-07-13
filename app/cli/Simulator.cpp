/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// Declaration
#include "Simulator.hpp"

// C++
#include <cctype>
#include <algorithm>
#include <thread>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/plugin/Context.hpp"

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
#include "cece/core/StringStream.hpp"
#include "cece/core/FileStream.hpp"
#endif

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
#include <stdio.h>
#if _WIN32
// Win32 have different names
#define popen _popen
#define pclose _pclose
#endif
#endif

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Context.hpp"
#  include "cece/simulator/Visualization.hpp"
#endif

#ifdef CECE_ENABLE_BOX2D_PHYSICS_DEBUG
#  include "cece/simulator/ConverterBox2D.hpp"
#endif

// CLI
#include "Image.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
const char* translateKey(int key) noexcept
{
    switch (key)
    {
    case GLFW_KEY_SPACE:        return " ";
    case GLFW_KEY_APOSTROPHE:   return "'";
    case GLFW_KEY_COMMA:        return ",";
    case GLFW_KEY_MINUS:        return "-";
    case GLFW_KEY_PERIOD:       return ".";
    case GLFW_KEY_SLASH:        return "/";
    case GLFW_KEY_0:            return "0";
    case GLFW_KEY_1:            return "1";
    case GLFW_KEY_2:            return "2";
    case GLFW_KEY_3:            return "3";
    case GLFW_KEY_4:            return "4";
    case GLFW_KEY_5:            return "5";
    case GLFW_KEY_6:            return "6";
    case GLFW_KEY_7:            return "7";
    case GLFW_KEY_8:            return "8";
    case GLFW_KEY_9:            return "9";
    case GLFW_KEY_SEMICOLON:    return ";";
    case GLFW_KEY_EQUAL:        return "=";
    case GLFW_KEY_A:            return "a";
    case GLFW_KEY_B:            return "b";
    case GLFW_KEY_C:            return "c";
    case GLFW_KEY_D:            return "d";
    case GLFW_KEY_E:            return "e";
    case GLFW_KEY_F:            return "f";
    case GLFW_KEY_G:            return "g";
    case GLFW_KEY_H:            return "h";
    case GLFW_KEY_I:            return "i";
    case GLFW_KEY_J:            return "j";
    case GLFW_KEY_K:            return "k";
    case GLFW_KEY_L:            return "l";
    case GLFW_KEY_M:            return "m";
    case GLFW_KEY_N:            return "n";
    case GLFW_KEY_O:            return "o";
    case GLFW_KEY_P:            return "p";
    case GLFW_KEY_Q:            return "q";
    case GLFW_KEY_R:            return "r";
    case GLFW_KEY_S:            return "s";
    case GLFW_KEY_T:            return "t";
    case GLFW_KEY_U:            return "u";
    case GLFW_KEY_V:            return "v";
    case GLFW_KEY_W:            return "w";
    case GLFW_KEY_X:            return "x";
    case GLFW_KEY_Y:            return "y";
    case GLFW_KEY_Z:            return "z";
    case GLFW_KEY_LEFT_BRACKET: return "[";
    case GLFW_KEY_BACKSLASH:    return "\\";
    case GLFW_KEY_RIGHT_BRACKET:return "]";
    case GLFW_KEY_GRAVE_ACCENT: return "`";
    case GLFW_KEY_ESCAPE:       return "escape";
    case GLFW_KEY_ENTER:        return "enter";
    case GLFW_KEY_TAB:          return "tab";
    case GLFW_KEY_BACKSPACE:    return "backspace";
    case GLFW_KEY_INSERT:       return "insert";
    case GLFW_KEY_DELETE:       return "delete";
    case GLFW_KEY_RIGHT:        return "right";
    case GLFW_KEY_LEFT:         return "left";
    case GLFW_KEY_DOWN:         return "down";
    case GLFW_KEY_UP:           return "up";
    case GLFW_KEY_PAGE_UP:      return "pageup";
    case GLFW_KEY_PAGE_DOWN:    return "pagedown";
    case GLFW_KEY_HOME:         return "home";
    case GLFW_KEY_END:          return "end";
    case GLFW_KEY_PAUSE:        return "pause";
    case GLFW_KEY_F1:           return "f1";
    case GLFW_KEY_F2:           return "f2";
    case GLFW_KEY_F3:           return "f3";
    case GLFW_KEY_F4:           return "f4";
    case GLFW_KEY_F5:           return "f5";
    case GLFW_KEY_F6:           return "f6";
    case GLFW_KEY_F7:           return "f7";
    case GLFW_KEY_F8:           return "f8";
    case GLFW_KEY_F9:           return "f9";
    case GLFW_KEY_F10:          return "f10";
    case GLFW_KEY_F11:          return "f11";
    case GLFW_KEY_F12:          return "f12";
    case GLFW_KEY_F13:          return "f13";
    case GLFW_KEY_F14:          return "f14";
    case GLFW_KEY_F15:          return "f15";
    case GLFW_KEY_F16:          return "f16";
    case GLFW_KEY_F17:          return "f17";
    case GLFW_KEY_F18:          return "f18";
    case GLFW_KEY_F19:          return "f19";
    case GLFW_KEY_F20:          return "f20";
    case GLFW_KEY_F21:          return "f21";
    case GLFW_KEY_F22:          return "f22";
    case GLFW_KEY_F23:          return "f23";
    case GLFW_KEY_F24:          return "f24";
    case GLFW_KEY_F25:          return "f25";
    case GLFW_KEY_KP_0:         return "0";
    case GLFW_KEY_KP_1:         return "1";
    case GLFW_KEY_KP_2:         return "2";
    case GLFW_KEY_KP_3:         return "3";
    case GLFW_KEY_KP_4:         return "4";
    case GLFW_KEY_KP_5:         return "5";
    case GLFW_KEY_KP_6:         return "6";
    case GLFW_KEY_KP_7:         return "7";
    case GLFW_KEY_KP_8:         return "8";
    case GLFW_KEY_KP_9:         return "9";
    case GLFW_KEY_KP_DECIMAL:   return ".";
    case GLFW_KEY_KP_DIVIDE:    return "/";
    case GLFW_KEY_KP_MULTIPLY:  return "*";
    case GLFW_KEY_KP_SUBTRACT:  return "-";
    case GLFW_KEY_KP_ADD:       return "+";
    case GLFW_KEY_KP_ENTER:     return "enter";
    case GLFW_KEY_KP_EQUAL:     return "=";
    }

    return "";
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
String strToLower(String str) noexcept
{
    std::transform(str.begin(), str.end(), str.begin(), [] (String::value_type c) {
        return tolower(c);
    });

    return str;
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

Simulator::Simulator(const Arguments& args)
    : m_simulationFile(args.simulationFile)
#ifdef CECE_ENABLE_RENDER
    , m_fullscreen(args.fullscreen)
#endif
{
    auto& manager = plugin::Manager::s();
    manager.addDirectories(args.pluginsDirectories);
    manager.loadAll();

    // Create plugins context
    plugin::Context context(manager.getRepository());
    context.importPlugin("xml");

    // Create simulation
    m_simulator.setSimulation(context.createSimulation(m_simulationFile, &args.parameters));

#ifdef CECE_ENABLE_RENDER
    if (args.windowWidth)
        m_windowWidth = args.windowWidth;

    if (args.windowHeight)
        m_windowHeight = args.windowHeight;

    const auto simViz = m_simulator.getSimulation()->getVisualization().isEnabled();

    // Decide if simulation should be visualized
    if (args.visualize)
        m_visualize = true;
    else if (!args.visualize)
        m_visualize = false;
    else
        m_visualize = simViz;

#  ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
    m_videoFileName = args.videoFileName;
#  endif
#endif
}

/* ************************************************************************ */

Simulator::~Simulator()
{
#ifdef CECE_ENABLE_RENDER
#ifdef CECE_CLI_ENABLE_RENDER_THREAD
    // Stop render thread
    m_renderThread.join();
#endif
#endif

    // Delete simulation
    m_simulator.setSimulation(nullptr);
#ifdef CECE_ENABLE_RENDER
    // Delete all released object before visualization cleanup
    m_simulator.getRenderContext().deleteReleasedObjects();
    cleanupVisualization();
#endif
}

/* ************************************************************************ */

ViewPtr<simulator::Simulation> Simulator::getSimulation() const noexcept
{
    return m_simulator.getSimulation();
}

/* ************************************************************************ */

void Simulator::init(AtomicBool& flag)
{
    // Initialize simulation
    m_simulator.getSimulation()->initialize(flag);

#ifdef CECE_ENABLE_RENDER
    if (isVisualized())
    {
        initVisualization();

        // Make the window's context current
        glfwMakeContextCurrent(m_window);

        // VSync
        glfwSwapInterval(1);

        // Initialize scene
        initScene();

#ifdef CECE_CLI_ENABLE_RENDER_THREAD
        // Disable current context
        glfwMakeContextCurrent(NULL);

        // Start render thread.
        m_renderThread = std::thread([&]() {

            // Make current context
            glfwMakeContextCurrent(m_window);

            while (!m_termination && !glfwWindowShouldClose(m_window))
            {
                if (!isVisualized())
                    continue;

                // Redraw scene
                if (!isPaused() || isForceRedraw())
                {
                    // Draw scene
                    draw();
                    swap();
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(16));
                }
            }

            glfwMakeContextCurrent(NULL);
        });
#endif
    }
#endif
}

/* ************************************************************************ */

void Simulator::step()
{
#ifdef CECE_ENABLE_RENDER
    if (isVisualized())
    {
        // Redraw scene
        if (!isPaused() || isForceRedraw())
        {
            // Update window title
            updateTitle();

#ifndef CECE_CLI_ENABLE_RENDER_THREAD
            // Draw scene
            draw();
            swap();
#endif
        }
#ifndef CECE_CLI_ENABLE_RENDER_THREAD
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
#endif

        /// Poll for and process events
        glfwPollEvents();
    }

    if (!isPaused())
        m_termination = !update();
#else
    m_termination = !update();
#endif
}

/* ************************************************************************ */

void Simulator::start(AtomicBool& flag)
{
    init(flag);

    // Update simulation
    while (flag && !isTerminationRequest())
        step();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::draw()
{
    Assert(isVisualized());

    // Get frame buffer size
    int width;
    int height;
    glfwGetFramebufferSize(m_window.get(), &width, &height);

    m_simulator.draw(width, height);

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
    // Capture image
    if (isVideoCaptured() && m_simulator.getSimulation()->getIteration() > 1)
        captureVideoFrame();
#endif
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::swap()
{
    Assert(isVisualized());

    // Swap buffers
    glfwSwapBuffers(m_window);
}
#endif

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
void Simulator::saveImage()
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
    oss << m_simulationFile.stem().string() << "_" << getSimulation()->getIteration() << ".png";

    const String filename = oss.str();

    // Get render context
    auto& context = m_simulator.getRenderContext();
    const auto size = context.getSize();

    // Get image data
    DynamicArray<unsigned char> data(size.getHeight() * size.getWidth() * 4);
    context.getData(data.data(), false);

    // Store image into file
    storeImage(filename, std::move(data), size.getWidth(), size.getHeight());

    Log::info("Image captured: ", filename);
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::onResize(int width, int height) noexcept
{
    // Store new window size.
    m_windowWidth = width;
    m_windowHeight = height;

    // Update visualization
    forceRedraw();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::onKeyPress(int key, int code, int action, int mods) noexcept
{
    constexpr float MOVE_COEFF = 5.0f;
    constexpr float ZOOM_COEFF = 1.1f;

    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    // Get camera
    auto& camera = m_simulator.getRenderContext().getCamera();

    switch (key)
    {
    default:
        if (updateLayers(key))
            forceRedraw();
        break;

    case GLFW_KEY_I:
    {
        camera.setZoom(camera.getZoom() * 1.0 / ZOOM_COEFF);
        forceRedraw();
        break;
    }

    case GLFW_KEY_O:
    {
        camera.setZoom(camera.getZoom() * ZOOM_COEFF);
        forceRedraw();
        break;
    }

    case GLFW_KEY_RIGHT:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * units::PositionVector{units::Length(-1), Zero}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_LEFT:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * units::PositionVector{units::Length(1), Zero}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_UP:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * units::PositionVector{Zero, units::Length(-1)}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_DOWN:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * units::PositionVector{Zero, units::Length(1)}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_R:
        // Reset camera
        setOptimalZoom();
        forceRedraw();
        break;

    case GLFW_KEY_Q:
        glfwSetWindowShouldClose(m_window, true);
        break;

    case GLFW_KEY_P:
        m_paused = !m_paused;
        break;

    case GLFW_KEY_S:
        if (m_paused)
        {
            update();
            // Update visualization
            forceRedraw();
        }
        break;

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
    case GLFW_KEY_C:
    {
        saveImage();
        break;
    }
#endif
    }
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::onMouseScroll(double xoffset, double yoffset) noexcept
{
    constexpr float ZOOM_COEFF = 1.1f;

    auto& camera = m_simulator.getRenderContext().getCamera();

    if (yoffset < 0)
    {
        camera.setZoom(camera.getZoom() * ZOOM_COEFF);
    }
    else if (yoffset > 0)
    {
        camera.setZoom(camera.getZoom() * 1.0 / ZOOM_COEFF);
    }

    // Update visualization
    forceRedraw();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::onMouseClick(int button, int action, int mods) noexcept
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            glfwGetCursorPos(m_window, &m_movePos.x(), &m_movePos.y());
        }
        else if (action == GLFW_RELEASE)
        {
            m_movePos = Zero;

            // Get position
            double x;
            double y;
            glfwGetCursorPos(m_window, &x, &y);
        }
    }
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::onMouseMove(double xpos, double ypos) noexcept
{
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
        return;

    // Get camera
    auto& camera = m_simulator.getRenderContext().getCamera();

    // Camera position
    auto pos = camera.getPosition();

    // Change vector
    const auto change = BasicVector<double, 2>(xpos, ypos) - m_movePos;

    pos += camera.getZoom() * units::PositionVector{
        units::Length(change.getX()),
        units::Length(-change.getY())
    };

    camera.setPosition(pos);

    m_movePos = BasicVector<double, 2>(xpos, ypos);

    // Update visualization
    forceRedraw();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
String Simulator::getTitle() const noexcept
{
    String title =
        APP_NAME
        " "
        "[" + m_simulationFile.filename().string() + "] "
        "(" + toString(getSimulation()->getIteration())
    ;

    if (!getSimulation()->hasUnlimitedIterations())
        title += "/" + toString(getSimulation()->getIterations());

    title += ")";

    return title;
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::updateTitle() noexcept
{
    auto title = getTitle();
    glfwSetWindowTitle(m_window, title.c_str());
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::initVisualization()
{
    if (!m_visualize)
        return;

    // Set callback
    // Nasty hack for Visual Studio... have issues with lambda to function conversion
    void (*errorCallback)(int, const char*) = [](int error, const char* desc) {
        Log::error(desc);
    };
    glfwSetErrorCallback(errorCallback);

    // Initialize the library
    if (!glfwInit())
        throw RuntimeException("Unable to initialize GLFW");

    // Get primary monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    // Monitor video mode
    const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

    // Fullscreen mode
    if (m_fullscreen)
    {
        // Borderless fullscreen mode
        if (!m_windowWidth && !m_windowHeight)
        {
            glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
        }

        if (!m_windowWidth)
            m_windowWidth = vidmode->width;

        if (!m_windowHeight)
            m_windowHeight = vidmode->height;
    }

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
    // Disable window resizing
    if (!m_videoFileName.empty())
        glfwWindowHint(GLFW_RESIZABLE, false);
#endif

    // Antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    const String title = getTitle();

    Assert(m_windowWidth);
    Assert(m_windowHeight);

    // Create a windowed mode window and its OpenGL context
    m_window = glfwCreateWindow(
        m_windowWidth,
        m_windowHeight,
        title.c_str(),
        m_fullscreen ? monitor : nullptr,
        nullptr
    );

    if (!m_window)
        throw RuntimeException("Unable to create rendering window");

    if (!m_fullscreen)
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

        // Resize window
        glfwSetWindowSize(m_window, m_windowWidth, m_windowHeight);
    }

    // Store this pointer
    glfwSetWindowUserPointer(m_window, this);

    // Antialiasing
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Set callbacks
    void (*windowSizeCallback)(GLFWwindow*, int, int) = [](GLFWwindow* win, int width, int height) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onResize(width, height);
    };
    glfwSetWindowSizeCallback(m_window, windowSizeCallback);

    void (*keyCallback)(GLFWwindow*, int, int, int, int) = [](GLFWwindow* win, int key, int code, int action, int mods) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onKeyPress(key, code, action, mods);
    };
    glfwSetKeyCallback(m_window, keyCallback);

    void (*scrollCallback)(GLFWwindow*, double, double) = [](GLFWwindow* win, double xoffset, double yoffset) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onMouseScroll(xoffset, yoffset);
    };
    glfwSetScrollCallback(m_window, scrollCallback);

    void (*mouseButtonCallback)(GLFWwindow*, int, int, int) = [](GLFWwindow* win, int button, int action, int mods) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onMouseClick(button, action, mods);
    };
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);

    void (*cursorPosCallback)(GLFWwindow*, double, double) = [](GLFWwindow* win, double xpos, double ypos) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onMouseMove(xpos, ypos);
    };
    glfwSetCursorPosCallback(m_window, cursorPosCallback);

    // Clear window
    glfwMakeContextCurrent(m_window);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);

#ifdef CECE_CLI_ENABLE_RENDER_THREAD
    glfwMakeContextCurrent(NULL);
#endif

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
    if (!m_videoFileName.empty())
        initVideoCapture(m_videoFileName);
#endif
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::cleanupVisualization() noexcept
{
#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
    terminateVideoCapture();
#endif

    if (!m_visualize)
        return;

    if (m_window)
        glfwDestroyWindow(m_window);

    glfwTerminate();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::initScene()
{
    // Get simulation
    auto simulation = m_simulator.getSimulation();

#ifdef CECE_ENABLE_BOX2D_PHYSICS_DEBUG

    m_physicsDebugger.SetFlags(
        render::PhysicsDebugger::e_shapeBit |
        render::PhysicsDebugger::e_centerOfMassBit |
        render::PhysicsDebugger::e_jointBit
    );
    simulation->getWorld().SetDebugDraw(&m_physicsDebugger);

    m_physicsDebugger.setScale(1.0 / simulator::ConverterBox2D::getInstance().getLengthCoefficient());
#endif

    // Initialize simulator
    m_simulator.drawInit(simulation->getVisualization().getBackgroundColor());

    // Initial zoom
    setOptimalZoom();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::setOptimalZoom()
{
    // Get frame buffer size
    int width;
    int height;
    glfwGetFramebufferSize(m_window.get(), &width, &height);

    auto size = m_simulator.getSimulation()->getWorldSize();
    auto& camera = m_simulator.getRenderContext().getCamera();
    camera.setPosition(Zero);
    camera.setZoom(
        std::max(size.getWidth() / width, size.getHeight() / height).value()
    );
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::forceRedraw()
{
    m_forceRedraw = true;
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
bool Simulator::updateLayers(int key)
{
    // Get visualization
    auto& visualization = getSimulation()->getVisualization();

    // Foreach layers
    for (auto& layer : visualization.getLayers())
    {
        if (translateKey(key) == strToLower(layer.getKey()))
        {
            layer.toggle();
            return true;
        }
    }

    return false;
}
#endif

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
void Simulator::initVideoCapture(const FilePath& fileName)
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
    filename += fileName.string();

    Log::info("Video output '", filename, "'");

    // Get frame buffer size
    int width;
    int height;
    glfwGetFramebufferSize(m_window.get(), &width, &height);

#if _WIN32
    if (std::system("WHERE ffmpeg") == 0)
#else
    if (std::system("which ffmpeg") == 0)
#endif
    {
        OutStringStream oss;
        oss <<
            "ffmpeg -r 60 -f rawvideo -pix_fmt rgba "
            "-s " << width << "x" << height << " "
            "-i - -threads 0 -vf vflip -y -q:v 1 "
            "" << filename
        ;

        Log::info("FFMPEG: ", oss.str());

        // Open video writer
        m_videoWriter = popen(oss.str().c_str(), "w");
    }
#if _WIN32
    else if (std::system("WHERE avconv") == 0)
#else
    else if (std::system("which avconv") == 0)
#endif
    {
        OutStringStream oss;
        oss <<
            "avconv -r 60 -f rawvideo -pix_fmt rgba "
            "-s " << width << "x" << height << " "
            "-i - -threads 0 -vf vflip -y -q:v 1 "
            "" << filename
        ;

        Log::info("AVCONV: ", oss.str());

        // Open video writer
        m_videoWriter = popen(oss.str().c_str(), "w");
    }

    // Disable window resizing
    if (!m_videoWriter)
        Log::warning("Unable to capture video: ffmpeg or avconv not found");
}
#endif

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
void Simulator::captureVideoFrame()
{
    Assert(isVideoCaptured());

    // Get render context
    auto& context = m_simulator.getRenderContext();
    const auto size = context.getSize();

    // Get image data
    DynamicArray<unsigned char> data(size.getHeight() * size.getWidth() * 4);
    context.getData(data.data(), true);

    // Store frame
    fwrite(data.data(), data.size(), 1, m_videoWriter);
}
#endif

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
void Simulator::terminateVideoCapture()
{
    if (m_videoWriter)
        pclose(m_videoWriter);
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
