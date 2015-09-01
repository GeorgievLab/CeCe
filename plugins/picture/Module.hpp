/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// OpenCV
#include <opencv2/opencv.hpp>

// Simulator
#include "core/String.hpp"
#include "core/FilePath.hpp"
#include "simulator/Module.hpp"
#include "render/Context.hpp"
#include "render/ImageData.hpp"

#if THREAD_SAFE
#include "core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace picture {

/* ************************************************************************ */

/**
 * @brief Module for storing pictures.
 */
class Module : public simulator::Module
{

// Public Accessors
public:


    /**
     * @brief Return pattern for generated file name.
     *
     * @return
     */
    const String& getFilePattern() const noexcept
    {
        return m_filePattern;
    }


    /**
     * @brief Get number of iteration when image should be saved.
     *
     * @return
     */
    unsigned int getSaveIteration() const noexcept
    {
        return m_saveIteration;
    }


// Public Mutators
public:


    /**
     * @brief Set file name pattern. Currently is supported only with
     * placeholder $1 that is replaced by step number.
     *
     * @param fileName Pattern to destination file.
     */
    void setFilePattern(String fileName) noexcept
    {
        m_filePattern = std::move(fileName);
    }


    /**
     * @brief Set number of step when image should be saved.
     *
     * @param iteration
     */
    void setSaveIteration(unsigned int iteration) noexcept
    {
        m_saveIteration = iteration;
    }


// Public Operations
public:


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;


// Protected Operations
protected:


    /**
     * @brief Save image.
     */
    void save(const FilePath& filename);


// Private Data Members
private:


    /// Pattern for file name.
    String m_filePattern = "image_$1.png";

    /// Save image each n-th step.
    unsigned int m_saveIteration = 1;

    /// Image data.
    cv::Mat m_image;

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
