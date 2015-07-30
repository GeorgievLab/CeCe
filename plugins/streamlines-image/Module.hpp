/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// OpenCV
#include <opencv2/opencv.hpp>

// Simulator
#include "core/String.hpp"

#ifdef ENABLE_RENDER
#include "render/Object.hpp"
#include "render/GridColor.hpp"
#endif

// Plugins
#include "plugins/streamlines/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines_image {

/* ************************************************************************ */

/**
 * @brief Module for storing pictures.
 */
class Module : public plugin::streamlines::Module
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


#ifdef ENABLE_RENDER
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

#ifdef ENABLE_RENDER
    /// Drawable object.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
