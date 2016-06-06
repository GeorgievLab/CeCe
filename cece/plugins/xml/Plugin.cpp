/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/plugin/Repository.hpp"
#include "cece/loader/Loader.hpp"
#include "cece/loader/FactoryManager.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"
#include "cece/simulator/DefaultSimulation.hpp"

// Plugin
#include "Configuration.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class XmlLoader : public loader::Loader
{

    /**
     * @brief Read simulation from input stream.
     *
     * @param repository Plugin repository.
     * @param is         Source stream.
     * @param filename   Source file name.
     * @param parameters Initialization parameters.
     *
     * @return Created simulation.
     */
    UniquePtr<Simulation> fromStream(const plugin::Repository& repository, InStream& source,
        const FilePath& filename, ViewPtr<const Parameters> parameters) const override
    {
        auto simulation = makeUnique<DefaultSimulation>(repository, filename);

        if (parameters)
            simulation->getParameters().append(*parameters);

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load(source);

        if (!result)
            throw RuntimeException("XML parse error: " + String(result.description()));

        // Create configuration
        const config::Configuration config(
            makeUnique<plugin::xml::ConfigImplementation>(doc.document_element()),
            &simulation->getParameters()
        );

        // Configure simulation
        simulation->loadConfig(config);

        return std::move(simulation);
    }


    /**
     * @brief Write simulation into output stream.
     *
     * @param os         Output stream.
     * @param simulation Source simulation.
     */
    void toStream(OutStream& os, const Simulation& simulation, const FilePath& filename) const override
    {
        pugi::xml_document doc;
        doc.append_child("simulation");

        // Create output configuration
        config::Configuration config(
            makeUnique<plugin::xml::ConfigImplementation>(doc.document_element()),
            nullptr
        );

        // Store configuration
        simulation.storeConfig(config);

        // Save configuration
        doc.save(os);
    }

};

/* ************************************************************************ */

class XmlApi : public plugin::Api
{

    /**
     * @brief On plugin load.
     *
     * @param repository Plugin repository.
     */
    void onLoad(plugin::Repository& repository) const override
    {
        repository.registerApi(this).
            registerLoader<XmlLoader>("xml").
            registerLoader<XmlLoader>("cece")
        ;
    }

};

/* ************************************************************************ */

CECE_DEFINE_PLUGIN(xml, XmlApi)

/* ************************************************************************ */
