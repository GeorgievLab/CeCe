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

#pragma once

/* ************************************************************************ */

// This must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/Units.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/python/Source.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Simple wrapper functor for Python code.
 */
class Initializer
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Initializer();


    /**
     * @brief Destructor.
     */
    ~Initializer();


// Public Operations
public:


    /**
     * @brief Initialize from source.
     *
     * @param source Source code.
     */
    void initSource(const String& source)
    {
        m_source.initSource(source);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const FilePath& filename)
    {
        m_source.initFile(filename);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Call program.
     *
     * @param simulation Current simulation.
     */
    void operator()(simulator::Simulation& simulation) const;


// Private Data Members
private:


    /// Source.
    Source m_source;

    /// Call function.
    Handle<PyObject> m_call;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
