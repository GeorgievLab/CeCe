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
#include "cece/core/UniquePtr.hpp"
#include "cece/core/String.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/PluginManager.hpp"
#include "cece/simulator/ModuleFactoryManager.hpp"

// Plugin
#include "cece/plugins/streamlines/Module.hpp"
#include "cece/plugins/streamlines/StoreState.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class StreamlinesApi : public PluginApi
{

    /**
     * @brief On plugin load.
     *
     * @param manager Plugin manager.
     */
    void onLoad(PluginManager& manager) override
    {
        manager.getModuleFactoryManager().createForModule<plugin::streamlines::Module>("streamlines");
        manager.getModuleFactoryManager().createForModule<plugin::streamlines::Module>("streamlines.store-state");
    }


    /**
     * @brief On plugin unload.
     *
     * @param manager Plugin manager.
     */
    void onUnload(PluginManager& manager) override
    {
        manager.getModuleFactoryManager().remove("streamlines");
        manager.getModuleFactoryManager().remove("streamlines.store-state");
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(streamlines, StreamlinesApi)

/* ************************************************************************ */
