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

// Simulator
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/DynamicArray.hpp"
#include "core/Map.hpp"
#include "core/ViewPtr.hpp"
#include "core/UniquePtr.hpp"
#include "simulator/SimulationLoader.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class SimulationLoader;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Manager of simulator loaders.
 */
class LoaderManager final
{

// Public Accessors
public:


    /**
     * @brief Return a list of loaders.
     *
     * @return An array of loader names.
     */
    static DynamicArray<String> getNames() noexcept;


    /**
     * @brief Check if loader is loaded.
     *
     * @param name Loader name.
     *
     * @return
     */
    static bool isAvailable(const StringView& name) noexcept
    {
        return s_loaders.find(name.getData()) != s_loaders.end();
    }


    /**
     * @brief Returns loader.
     *
     * @param name Loader name.
     *
     * @return Pointer to loader or nullptr, if loader is not loaded.
     */
    static ViewPtr<SimulationLoader> get(const StringView& name) noexcept
    {
        // Try to find library
        auto it = s_loaders.find(name.getData());

        if (it == s_loaders.end())
            return nullptr;

        return std::get<1>(*it).get();
    }


    /**
     * @brief Create simulation from file.
     *
     * @param filepath Path to file.
     *
     * @return
     */
    static UniquePtr<Simulation> create(const FilePath& filepath);


// Public Mutators
public:


    /**
     * @brief Add new loader.
     *
     * @param name   Loader name/extension.
     * @param loader New loader.
     */
    static void add(const StringView& name, UniquePtr<SimulationLoader> loader) noexcept
    {
        s_loaders[name.getData()] = std::move(loader);
    }


    /**
     * @brief Add new loader.
     *
     * @tparam T Loader type.
     *
     * @param name Loader name/extension.
     */
    template<typename T>
    static void add(const StringView& name) noexcept
    {
        add(name, makeUnique<T>());
    }


// Private Data Members
private:

    /// Loaders.
    static Map<String, UniquePtr<SimulationLoader>> s_loaders;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
