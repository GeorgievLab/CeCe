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

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
