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
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/plugin/Context.hpp"

// Plugin
#include "cece/plugins/streamlines-channel/Module.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

class StreamlinesChannelApi : public plugin::Api
{
public:


    /**
     * @brief Returns a list of required plugins.
     *
     * @return
     */
    DynamicArray<String> requiredPlugins() const override
    {
        return {"streamlines"};
    }


    /**
     * @brief On plugin load.
     *
     * @param context Plugin context.
     */
    void onLoad(plugin::Context& context) override
    {
        // Unregister previous streamlines module
        context.unregisterModule("streamlines");
        context.registerModule<plugin::streamlines_channel::Module>("streamlines");
    }


    /**
     * @brief On plugin unload.
     *
     * @param context Plugin context.
     */
    void onUnload(plugin::Context& context) override
    {
        context.unregisterModule("streamlines");
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(streamlines_channel, StreamlinesChannelApi)

/* ************************************************************************ */
