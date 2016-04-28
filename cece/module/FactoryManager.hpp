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
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/FactoryManager.hpp"
#include "cece/module/Factory.hpp"

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Module factory manager.
 */
class CECE_EXPORT FactoryManager : public core::FactoryManager<Factory>
{

// Public Mutators
public:


    /**
     * @brief Register a new factory for specified module.
     *
     * @param name Factory name.
     */
    template<typename ModuleType>
    void createForModule(String name) noexcept
    {
        create<FactoryTyped<ModuleType>>(std::move(name));
    }


// Public Operations
public:


    /**
     * @brief Create module by name.
     *
     * @param name       Factory name.
     * @param simulation Owning simulation.
     *
     * @return Created module.
     *
     * @throw FactoryNotFoundException In case of factory with given name doesn't exists.
     */
    UniquePtr<Module> createModule(StringView name, simulator::Simulation& simulation) const;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
