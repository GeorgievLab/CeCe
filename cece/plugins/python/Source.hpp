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
#include "cece/core/String.hpp"
#include "cece/core/FilePath.hpp"

// Plugin
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/View.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Simple wrapper functor for Python code.
 */
class Source
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Source();


// Public Accessors
public:


    /**
     * @brief Returns if source is initialized.
     *
     * @return
     */
    bool isInitialized() const noexcept
    {
        return m_initialized;
    }


// Public Accessors
public:


    /**
     * @brief Returns Python object of required name.
     *
     * @param name Function name.
     *
     * @return Pointer to function or nullptr.
     */
    Handle<PyObject> getFunction(const char* name) const noexcept;


    /**
     * @brief Returns Python object of required name.
     *
     * @param name Function name.
     *
     * @return Pointer to function or nullptr.
     */
    Handle<PyObject> getFunction(const String& name) const noexcept
    {
        return getFunction(name.c_str());
    }


// Public Operations
public:


    /**
     * @brief Initialize from source.
     *
     * @param source Source code.
     */
    void initSource(const String& source);


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const FilePath& filename);


// Private Data Members
private:

    /// If source is initialized.
    bool m_initialized = false;

    /// Main module dictionary.
    Handle<PyObject> m_dictionary;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
