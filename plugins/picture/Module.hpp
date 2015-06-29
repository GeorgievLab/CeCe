
#pragma once

/* ************************************************************************ */

// Magick++
#include <Magick++.h>

// Simulator
#include "simulator/Module.hpp"
#include "render/Context.hpp"

/* ************************************************************************ */

namespace plugin {
namespace picture {

/* ************************************************************************ */

/**
 * @brief Module for storing pictures.
 */
class Module : public simulator::Module
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module();


// Public Accessors
public:


    /**
     * @brief Return pattern for generated file name.
     *
     * @return
     */
    const std::string& getFileName() const noexcept
    {
        return m_fileName;
    }


    /**
     * @brief Get number of step when image should be saved.
     *
     * @return
     */
    unsigned int getEachStep() const noexcept
    {
        return m_eachStep;
    }


// Public Mutators
public:


    /**
     * @brief Set file name pattern. Currently is supported only with
     * placeholder $1 that is replaced by step number.
     *
     * @param fileName
     */
    void setFileName(std::string fileName) noexcept
    {
        m_fileName = std::move(fileName);
    }


    /**
     * @brief Set number of step when image should be saved.
     *
     * @param eachStep
     */
    void setEachStep(unsigned int eachStep) noexcept
    {
        m_eachStep = eachStep;
    }


// Public Operations
public:


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::Configuration& config) override;


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


// Private Data Members
private:


    /// Pattern for file name.
    std::string m_fileName = "image_%ul.png";

    /// Save image each n-th step.
    unsigned int m_eachStep = 1;

    /// Last image.
    Magick::Image m_image;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
