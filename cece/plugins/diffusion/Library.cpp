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

// CeCe
#include "cece/simulator/Simulation.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/ModuleFactoryManager.hpp"

// Plugin
#include "cece/plugins/diffusion/Module.hpp"
#include "cece/plugins/diffusion/Generator.hpp"
#include "cece/plugins/diffusion/StoreState.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class DiffusionApi : public PluginApi
{

    /**
     * @brief On plugin load.
     */
    void onLoad() override
    {
        cece::simulator::ModuleFactoryManager::s().createForModule<plugin::diffusion::Module>("diffusion");
        cece::simulator::ModuleFactoryManager::s().createForModule<plugin::diffusion::Generator>("diffusion.generator");
        cece::simulator::ModuleFactoryManager::s().createForModule<plugin::diffusion::StoreState>("diffusion.store-state");
    }


    /**
     * @brief On plugin unload.
     */
    void onUnload() override
    {
        cece::simulator::ModuleFactoryManager::s().remove("diffusion.store-state");
        cece::simulator::ModuleFactoryManager::s().remove("diffusion.generator");
        cece::simulator::ModuleFactoryManager::s().remove("diffusion");
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(diffusion, DiffusionApi)

/* ************************************************************************ */
