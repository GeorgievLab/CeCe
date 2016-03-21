/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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

// CeCe
#include "cece/plugin/Manager.hpp"

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
#include "cece/core/StringStream.hpp"
#include "cece/core/FileStream.hpp"
#endif

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Context.hpp"
#endif

// CLI
#include "Image.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

Simulator::Simulator(const Arguments& args)
    : m_simulationFile(args.simulationFile)
#ifdef CECE_ENABLE_RENDER
    , m_fullscreen(args.fullscreen)
#endif
{
    auto& manager = plugin::Manager::s();
    manager.addDirectories(args.pluginsDirectories);

    // Load initial loader
    manager.load("xml");

    // Create simulation
    m_simulator.setSimulation(manager.getContext().createSimulation(m_simulationFile, &args.parameters));

#ifdef CECE_ENABLE_RENDER
    if (args.windowWidth)
        m_windowWidth = args.windowWidth;

    if (args.windowHeight)
        m_windowHeight = args.windowHeight;

    const auto simViz = m_simulator.getSimulation()->isVisualized();

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

void Simulator::init(AtomicBool& termFlag)
{
    // Initialize simulation
    m_simulator.getSimulation()->initialize(termFlag);

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
    }
#endif
}

/* ************************************************************************ */

void Simulator::step()
{
#ifdef CECE_ENABLE_RENDER
    if (!isPaused())
        m_termination = !update();

    if (isVisualized())
    {
        // Redraw scene
        if (!isPaused() || isForceRedraw())
        {
            // Draw scene
            draw();
            swap();
        }

        /// Poll for and process events
        glfwPollEvents();
    }
#else
    m_termination = !update();
#endif
}

/* ************************************************************************ */

void Simulator::start(AtomicBool& termFlag)
{
    init(termFlag);

    // Update simulation
    while (!termFlag && !isTerminationRequest())
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
    if (isVideoCaptured() && m_simulator.getSimulation()->getIteration() > 3)
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
    constexpr float MOVE_COEFF = 5;
    constexpr float ZOOM_COEFF = 1.1;

    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    // Get camera
    auto& camera = m_simulator.getRenderContext().getCamera();

    switch (key)
    {
    default:
        // Nothing
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
            MOVE_COEFF * PositionVector{units::Length(-1), Zero}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_LEFT:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * PositionVector{units::Length(1), Zero}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_UP:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * PositionVector{Zero, units::Length(-1)}
        );
        forceRedraw();
        break;
    }

    case GLFW_KEY_DOWN:
    {
        camera.setPosition(camera.getPosition() +
            MOVE_COEFF * PositionVector{Zero, units::Length(1)}
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

#ifdef CECE_ENABLE_BOX2D_PHYSICS_DEBUG
    case GLFW_KEY_D:
        m_simulator.getSimulation()->setDrawPhysics(!m_simulator.getSimulation()->isDrawPhysics());
        forceRedraw();
        break;
#endif

    case GLFW_KEY_V:
    {
        auto streamlines = getSimulation()->getModule("streamlines");

        if (streamlines)
        {
            if (streamlines->getDrawFlags() & 0x1)
                streamlines->setDrawFlags(streamlines->getDrawFlags() & ~0x1);
            else
                streamlines->setDrawFlags(streamlines->getDrawFlags() | 0x1);

            forceRedraw();
        }

        break;
    }

    case GLFW_KEY_B:
    {
        auto streamlines = getSimulation()->getModule("streamlines");

        if (streamlines)
        {
            if (streamlines->getDrawFlags() & 0x2)
                streamlines->setDrawFlags(streamlines->getDrawFlags() & ~0x2);
            else
                streamlines->setDrawFlags(streamlines->getDrawFlags() | 0x2);

            forceRedraw();
        }

        break;
    }

    case GLFW_KEY_N:
    {
        auto streamlines = getSimulation()->getModule("streamlines");

        if (streamlines)
        {
            if (streamlines->getDrawFlags() & 0x4)
                streamlines->setDrawFlags(streamlines->getDrawFlags() & ~0x4);
            else
                streamlines->setDrawFlags(streamlines->getDrawFlags() | 0x4);

            forceRedraw();
        }

        break;
    }
    }
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulator::onMouseScroll(double xoffset, double yoffset) noexcept
{
    constexpr float ZOOM_COEFF = 1.1;

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

    pos += camera.getZoom() * PositionVector{
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
void Simulator::initVisualization()
{
    if (!m_visualize)
        return;

    // Set callback
    glfwSetErrorCallback(+[](int error, const char* desc) {
        Log::error(desc);
    });

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

    const String title = APP_NAME " simulator [" + m_simulationFile.filename().string() + "]";

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
    glfwSetWindowSizeCallback(m_window, +[](GLFWwindow* win, int width, int height) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onResize(width, height);
    });

    glfwSetKeyCallback(m_window, +[](GLFWwindow* win, int key, int code, int action, int mods) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onKeyPress(key, code, action, mods);
    });

    glfwSetScrollCallback(m_window, +[](GLFWwindow* win, double xoffset, double yoffset) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onMouseScroll(xoffset, yoffset);
    });

    glfwSetMouseButtonCallback(m_window, +[](GLFWwindow* win, int button, int action, int mods) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onMouseClick(button, action, mods);
    });

    glfwSetCursorPosCallback(m_window, +[](GLFWwindow* win, double xpos, double ypos) {
        reinterpret_cast<Simulator*>(glfwGetWindowUserPointer(win))->onMouseMove(xpos, ypos);
    });

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

    m_physicsDebugger.setScale(1.0 / simulation->getConverter().getLengthCoefficient());
#endif

    // Initialize simulator
    m_simulator.drawInit(simulation->getBackgroundColor());

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

    if (system("which ffmpeg") == 0)
    {
        OutStringStream oss;
        oss <<
            "ffmpeg -r 60 -f rawvideo -pix_fmt rgba "
            "-s " << width << "x" << height << " "
            "-i - -threads 0 -vf vflip -y "
            "" << filename
        ;

        Log::info("FFMPEG: ", oss.str());

        // Open video writer
        m_videoWriter = popen(oss.str().c_str(), "w");
    }
    else if (system("which avconv") == 0)
    {
        OutStringStream oss;
        oss <<
            "avconv -r 60 -f rawvideo -pix_fmt rgba "
            "-s " << width << "x" << height << " "
            "-i - -threads 0 -vf vflip -y "
            "" << filename
        ;

        Log::info("AVCONV: ", oss.str());

        // Open video writer
        m_videoWriter = popen(oss.str().c_str(), "w");
    }

    // Disable window resizing
    if (!m_videoWriter)
        Log::warning("Unable to capture video");
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
