
#pragma once

#if !ENABLE_RENDER
#error Unable to build module without ENABLE_RENDER
#endif

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "render/Context.hpp"

/* ************************************************************************ */

namespace module {
namespace picture {

/* ************************************************************************ */

/**
 * @brief Module for storing pictures.
 */
class Module : public simulator::Module
{

    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
