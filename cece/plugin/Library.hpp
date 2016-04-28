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

// CeCe
#include "cece/export.hpp"
#include "cece/core/String.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/plugin/Config.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

class Api;
class Context;

/* ************************************************************************ */

/**
 * @brief Helper class for wrapping plugin.
 */
class CECE_EXPORT Library final
{

// Public Constants
public:


    /// Library file prefix
    static const String FILE_PREFIX;

    /// Library file extension
    static const String FILE_EXTENSION;


// Public Types
public:


    /// Create API function pointer type.
    using CreateFn = Api* (*)();

    /// Returns plugin configuration.
    using GetConfigFn = Config* (*)();


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Plugin name.
     * @param api  Plugin builtin api.
     */
    Library(String name, UniquePtr<Api> api);


    /**
     * @brief Constructor.
     *
     * @param name Plugin name.
     * @param path Path to plugin.
     */
    Library(String name, FilePath path);


    /**
     * @brief Destructor.
     */
    ~Library();


// Public Accessors
public:


    /**
     * @brief Returns plugin name.
     *
     * @return
     */
    const String& getName() const noexcept
    {
        return m_name;
    }


    /**
     * @brief Returns API object.
     *
     * @return
     */
    ViewPtr<Api> getApi() const noexcept
    {
        return m_api;
    }


// Private Operations
private:


    /**
     * @brief Check configuration file.
     *
     * @param config
     *
     * @throw
     */
    void checkConfig(Config* config);


// Private Data Members
private:

    /// Plugin name.
    String m_name;

    /// Implementation
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Library API.
    UniquePtr<Api> m_api;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
