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

// Simulator
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/PluginManager.hpp"

// Plugin
#include "Configuration.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class XmlApi : public PluginApi
{

    String getLoaderExtension() const noexcept override
    {
        return "xml";
    }

    UniquePtr<Simulation> fromStream(InStream& source, const FilePath& filename) const override
    {
        auto simulation = makeUnique<Simulation>();

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load(source);

        if (!result)
            throw RuntimeException("XML parse error: " + String(result.description()));

        {
            // Register file path as module library
#if __linux__
            char buffer[1024];
            strncpy(buffer, filename.c_str(), sizeof(buffer));

            PluginManager::s().addDirectory(dirname(buffer));
#endif
        }

        // Create configuration
        const Configuration config(
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

DEFINE_PLUGIN(xml, XmlApi)

/* ************************************************************************ */
