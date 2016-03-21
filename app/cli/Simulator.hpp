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

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

// CLI config
#include "config.hpp"

/* ************************************************************************ */

// C++
#include <atomic>

// GLFW
#ifdef CECE_ENABLE_RENDER
#include <GLFW/glfw3.h>
#endif

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/simulator/Simulator.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
#include "cece/core/StringStream.hpp"
#include "cece/core/FileStream.hpp"
#endif

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
#include <cstdio>
#endif

#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)
#  include "cece/render/PhysicsDebugger.hpp"
#endif

// CLI
#include "Arguments.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

/**
 * @brief CLI simulator.
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
    explicit Simulator(const Arguments& args);


    /**
     * @brief Destructor.
     */
    ~Simulator();


// Public Accessors
public:


    /**
     * @brief Returns current simulation.
     *
     * @return
     */
    ViewPtr<simulator::Simulation> getSimulation() const noexcept;


    /**
     * @brief Returns if simulation is visualized.
     *
     * @return
     */
    bool isVisualized() const noexcept
    {
        return m_visualize;
    }


    /**
     * @brief Returns if simulation is paused.
     *
     * @return
     */
    bool isPaused() const noexcept
    {
        return m_paused;
    }


    /**
     * @brief If termination request has been sent.
     *
     * @return
     */
    bool isTerminationRequest() const noexcept
    {
#ifdef CECE_ENABLE_RENDER
        return m_termination || glfwWindowShouldClose(m_window);
#else
        return m_termination;
#endif
    }


// Public Operations
public:


    /**
     * @brief Initialize simulation.
     *
     * @param termFlag Termination flag.
     */
    void init(AtomicBool& termFlag);


    /**
     * @brief Perform simulation step.
     */
    void step();


    /**
     * @brief Start simulation.
     *
     * @param termFlag Termination flag.
     */
    void start(AtomicBool& termFlag);


    /**
     * @brief Update simulation.
     *
     * @return Update result.
     */
    bool update()
    {
        return m_simulator.update();
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Draw scene.
     */
    void draw();


    /**
     * @brief Swap render buffers.
     */
    void swap();


#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE

    /**
     * @brief Save image of current state.
     */
    void saveImage();

#endif


    /**
     * @brief On window resize.
     *
     * @param w
     * @param h
     */
    void onResize(int w, int h) noexcept;


    /**
     * @brief On key press.
     *
     * @param key
     * @param code
     * @param action
     * @param mods
     */
    void onKeyPress(int key, int code, int action, int mods) noexcept;


    /**
     * @brief On mouse scroll.
     *
     * @param xoffset
     * @param yoffset
     */
    void onMouseScroll(double xoffset, double yoffset) noexcept;


    /**
     * @brief On mouse button click.
     *
     * @param button
     * @param action
     * @param mods
     */
    void onMouseClick(int button, int action, int mods) noexcept;


    /**
     * @brief On mouse move.
     *
     * @param xpos
     * @param ypos
     */
    void onMouseMove(double xpos, double ypos) noexcept;

#endif


// Private Accessors
private:


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns if visualization needs to be updated.
     *
     * @return
     */
    bool isForceRedraw() const noexcept
    {
        return m_forceRedraw;
    }

#endif


// Private Operations
private:


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Init visualization.
     */
    void initVisualization();


    /**
     * @brief Cleanup visualization.
     */
    void cleanupVisualization() noexcept;


    /**
     * @brief Initialize rendering scene.
     */
    void initScene();


    /**
     * @brief Calculate zoom to render scene in whole window.
     */
    void setOptimalZoom();


    /**
     * @brief Set a request to redraw.
     */
    void forceRedraw();


#ifdef CONFIG_CLI_ENABLE_VIDEO_CAPTURE

    /**
     * @brief Initialize video capturing.
     *
     * @param fileName
     */
    void initVideoCapture(const FilePath& fileName);


    /**
     * @brief If video is captured.
     *
     * @return
     */
    bool isVideoCaptured() const noexcept
    {
        return m_videoWriter != nullptr;
    }


    /**
     * @brief Capture video frame.
     */
    void captureVideoFrame();


    /**
     * @brief Terminate video capturing.
     */
    void terminateVideoCapture();

#endif
#endif

// Private Data Members
private:

#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)
    // Physics engine debug draw.
    render::PhysicsDebugger m_physicsDebugger;
#endif

    /// Termination request flag.
    bool m_termination = false;

    /// If simulation is paused.
    bool m_paused = false;

    /// Path to simulation file.
    FilePath m_simulationFile;

    // Simulator
    simulator::Simulator m_simulator;

#ifdef CECE_ENABLE_RENDER
    // If simulation should be rendered.
    bool m_visualize = true;

    /// Window fullscreen.
    bool m_fullscreen = false;

    /// Window width.
    unsigned int m_windowWidth = 800;

    /// Window height.
    unsigned int m_windowHeight = 600;

    /// Render window.
    ViewPtr<GLFWwindow> m_window;

    /// Force redraw flag.
    bool m_forceRedraw = false;

    /// Last position for move cursor.
    BasicVector<double, 2> m_movePos;
#endif

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE

    /// Path to output video file.
    FilePath m_videoFileName;

    /// Video writer.
    FILE* m_videoWriter = nullptr;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
