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
#include "simulator/Module.hpp"

#if ENABLE_RENDER
#  include "render/Object.hpp"
#  include "render/GridColor.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace background {

/* ************************************************************************ */

/**
 * @brief Module for rendering background image.
 */
class Module : public simulator::Module
{


// Public Operations
public:


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:

    /// Source image.
    cv::Mat m_img;

#if ENABLE_RENDER
    /// Drawable object.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
