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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Units.hpp"

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }

#ifdef CECE_ENABLE_RENDER
namespace cece { namespace render { class Context; } }
#endif

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Container for modules.
 */
class Container
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Container();


// Public Accessors
public:


    /**
     * @brief Returns if module with given name exists.
     *
     * @param name Module name.
     *
     * @return
     */
    bool exists(StringView name) const noexcept;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Module name.
     *
     * @return Pointer to module. Can be nullptr.
     */
    ViewPtr<Module> get(StringView name) const noexcept;


// Public Mutators
public:


    /**
     * @brief Store or replace module.
     *
     * @param name   Module name.
     * @param module Module object.
     *
     * @return Pointer to added module.
     */
    ViewPtr<Module> add(String name, UniquePtr<Module> module);


    /**
     * @brief Clear container.
     */
    void clear();


// Public Operations
public:


    /**
     * @brief Update all modules.
     *
     * @param simulation Simulation object.
     * @param dt         Time step.
     */
    void update(simulator::Simulation& simulation, units::Time dt);


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render modules sorted by z-order.
     *
     * @param simulation Current simulation.
     * @param context    Rendering context.
     */
    void draw(const simulator::Simulation& simulation, render::Context& context);

#endif


// Private Data Members
private:

    /// Stored modules.
    DynamicArray<Pair<String, UniquePtr<Module>>> m_modules;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
