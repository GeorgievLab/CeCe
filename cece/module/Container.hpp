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
#include "cece/core/ViewPtr.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Atomic.hpp"
#include "cece/core/PtrNamedContainer.hpp"

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
 *
 * @todo Cache sorted list of modules.
 */
class Container : public PtrNamedContainer<Module>
{

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

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
