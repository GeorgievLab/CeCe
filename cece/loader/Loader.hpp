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

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/String.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/ViewPtr.hpp"

/* ************************************************************************ */

namespace cece { inline namespace core { class Parameters; } }
namespace cece { namespace simulator { class Simulation; } }
namespace cece { namespace plugin { class Context; } }

/* ************************************************************************ */

namespace cece {
namespace loader {

/* ************************************************************************ */

/**
 * @brief Simulation loader base class.
 */
class CECE_EXPORT Loader
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Loader()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Create a new simulation from source file.
     *
     * @param context    Plugin context.
     * @param filename   Path to source file.
     * @param parameters Initialization parameters.
     *
     * @return Pointer to created simulation.
     */
    virtual UniquePtr<simulator::Simulation> fromFile(plugin::Context& context,
        const FilePath& filename, ViewPtr<const Parameters> parameters = nullptr) const;


    /**
     * @brief Create a new simulation from source code.
     *
     * @param source   String with source.
     * @param filename Path to source file.
     *
     * @return Pointer to created simulation.
     */
    virtual UniquePtr<simulator::Simulation> fromSource(plugin::Context& context,
        const String& source, const FilePath& filename = "<source>", ViewPtr<const Parameters> parameters = nullptr) const;


    /**
     * @brief Store simulation into file.
     *
     * @param simulation Source simulation.
     * @param filename   Path to source file.
     */
    virtual void toFile(const simulator::Simulation& simulation,
        const FilePath& filename) const;


    /**
     * @brief Convert simulation into source code.
     *
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    virtual String toSource(const simulator::Simulation& simulation,
        const FilePath& filename = "<source>") const;


    /**
     * @brief Read simulation from input stream.
     *
     * @param context    Plugin context.
     * @param is         Source stream.
     * @param filename   Source file name.
     * @param parameters Initialization parameters.
     *
     * @return Created simulation.
     */
    virtual UniquePtr<simulator::Simulation> fromStream(plugin::Context& context,
        InStream& is, const FilePath& filename = "<stream>", ViewPtr<const Parameters> parameters = nullptr) const = 0;


    /**
     * @brief Write simulation into output stream.
     *
     * @param os         Output stream.
     * @param simulation Source simulation.
     */
    virtual void toStream(OutStream& os, const simulator::Simulation& simulation,
        const FilePath& filename = "<stream>") const = 0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
