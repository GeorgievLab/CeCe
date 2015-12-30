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

// Linux
#if __linux__
#include <libgen.h>
#endif

// pugixml
#include "pugixml/pugixml.hpp"

// CeCe
#include "cece/core/UniquePtr.hpp"
#include "cece/core/String.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/plugin/Context.hpp"
#include "cece/loader/Loader.hpp"
#include "cece/loader/FactoryManager.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "Configuration.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class XmlLoader : public loader::Loader
{

    UniquePtr<Simulation> fromStream(plugin::Context& context, InStream& source,
        const FilePath& filename) const override
    {
        auto simulation = makeUnique<Simulation>(context);

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load(source);

        if (!result)
            throw RuntimeException("XML parse error: " + String(result.description()));

        // Create configuration
        const config::Configuration config(
            makeUnique<plugin::xml::ConfigImplementation>(doc.document_element()),
            filename
        );

        // Configure simulation
        simulation->configure(config);

        return simulation;
    }


    void toStream(OutStream& os, const Simulation& simulation, const FilePath& filename) const override
    {
        // TODO: implement
    }

};

/* ************************************************************************ */

class XmlApi : public plugin::Api
{

    /**
     * @brief On plugin load.
     *
     * @param context Plugin context.
     */
    void onLoad(plugin::Context& context) override
    {
        context.registerLoader<XmlLoader>("xml");
        context.registerLoader<XmlLoader>("cece");
    }


    /**
     * @brief On plugin unload.
     *
     * @param context Plugin context.
     */
    void onUnload(plugin::Context& context) override
    {
        context.unregisterLoader("cece");
        context.unregisterLoader("xml");
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(xml, XmlApi)

/* ************************************************************************ */
