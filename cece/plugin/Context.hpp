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
#include "cece/core/FilePath.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Parameters.hpp"

/// @deprecated
#include "cece/object/Object.hpp"

/* ************************************************************************ */

namespace cece {
    namespace init      { class Initializer; }
    namespace module    { class Module; }
    //namespace object    { class Object; }
    namespace object    { class Type; }
    namespace program   { class Program; }
    namespace simulator { class Simulation; }
}

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

class Api;
class Repository;

/* ************************************************************************ */

/**
 * @brief Plugins context for simulation.
 */
class Context
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param repository Plugins repository.
     */
    explicit Context(const Repository& repository) noexcept
        : m_repository(repository)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns context repository.
     *
     * @return
     */
    const Repository& getRepository() const noexcept
    {
        return m_repository;
    }


    /**
     * @brief Returns map of imported plugins.
     *
     * @return
     */
    const Map<String, ViewPtr<const Api>>& getImported() const noexcept
    {
        return m_plugins;
    }


    /**
     * @brief Returns if plugin is already imported.
     *
     * @param name Plugin name.
     *
     * @return
     */
    bool isImported(StringView name) const noexcept;


// Public Operations
public:


    /**
     * @brief Import plugin.
     *
     * @param name Plugin name.
     *
     * @return Plugin API.
     */
    ViewPtr<const Api> importPlugin(StringView name);


    /**
     * @brief Remove plugin.
     *
     * @param name Plugin name.
     *
     * @return Plugin API.
     */
    ViewPtr<const Api> removePlugin(StringView name);


    /**
     * @brief Create a simulation from file.
     *
     * @param filepath   Path to file.
     * @param parameters Optional initialization parameters.
     *
     * @return Pointer to simulation.
     *
     * @throw In case of missing file or error in simulation file.
     */
    UniquePtr<simulator::Simulation> createSimulation(const FilePath& filepath, ViewPtr<const Parameters> parameters = nullptr) const;


    /**
     * @brief Create a simulation from source.
     *
     * @param type   Type of used loader.
     * @param source Source code.
     *
     * @return Pointer to simulation.
     *
     * @throw In case of missing file or error in simulation file.
     */
    UniquePtr<simulator::Simulation> createSimulation(StringView type, StringView source) const;


    /**
     * @brief Create a simulation from source.
     *
     * @param type     Type of used loader.
     * @param source   Source code.
     * @param filepath Path to file.
     *
     * @return Pointer to simulation.
     *
     * @throw In case of missing file or error in simulation file.
     */
    UniquePtr<simulator::Simulation> createSimulation(StringView type, StringView source, const FilePath& filepath) const;


    /**
     * @brief Create an initializer of given type name.
     *
     * @param typeName Type of required initializer.
     *
     * @return Pointer to created initializer.
     */
    UniquePtr<init::Initializer> createInitializer(StringView typeName) const;


    /**
     * @brief Create a module of given type name.
     *
     * @param typeName   Type of required module.
     * @param simulation Simulation object.
     *
     * @return Pointer to created module.
     */
    UniquePtr<module::Module> createModule(StringView typeName, simulator::Simulation& simulation) const;


    /**
     * @brief Create an object of given type name.
     *
     * @param typeName   Type of required object.
     * @param simulation Simulation object.
     * @param type       Type of created object.
     *
     * @return Pointer to created object.
     */
    UniquePtr<object::Object> createObject(StringView typeName, simulator::Simulation& simulation, object::Object::Type type) const;


    /**
     * @brief Create a program of given type name.
     *
     * @param typeName Type of required program.
     *
     * @return Pointer to created program.
     */
    UniquePtr<program::Program> createProgram(StringView typeName) const;


// Private Data Members
private:

    /// Plugins repository.
    const Repository& m_repository;

    /// Imported plugins.
    Map<String, ViewPtr<const Api>> m_plugins;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
