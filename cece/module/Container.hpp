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
#include "cece/core/Atomic.hpp"

/* ************************************************************************ */

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

// Public Structures
public:


    /**
     * @brief Container record.
     */
    struct Record
    {
        /// Module name.
        String name;

        /// Pointer to module.
        UniquePtr<Module> module;
    };


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


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator begin() const noexcept
    {
        return m_modules.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator cbegin() const noexcept
    {
        return m_modules.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator end() const noexcept
    {
        return m_modules.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator cend() const noexcept
    {
        return m_modules.cend();
    }


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
     * @brief Initialize all modules.
     *
     * @param termFlag Termination flag.
     */
    void init(AtomicBool& termFlag);


    /**
     * @brief Update all modules.
     */
    void update();


    /**
     * @brief Terminate all modules.
     */
    void terminate();


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render modules sorted by z-order.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context);

#endif

// Protected Operations
protected:


    /**
     * @brief Returns sorted list of modules by priority.
     *
     * @return
     */
    DynamicArray<ViewPtr<Module>> getSortedListAsc() const noexcept;


    /**
     * @brief Returns sorted list of modules by priority.
     *
     * @return
     */
    DynamicArray<ViewPtr<Module>> getSortedListDesc() const noexcept;


// Private Data Members
private:

    /// Stored modules.
    DynamicArray<Record> m_modules;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
