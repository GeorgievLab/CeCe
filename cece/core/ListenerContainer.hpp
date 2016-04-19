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
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Template container for storing listeners and invoking it's member
 * functions at once.
 *
 * Basic idea of this container is simplify code to call same member
 * function of multiple listeners. Without this container a programmer must
 * write code to for each all listener stored in some other container and
 * call listener's member function. This class replaces for each part with
 * only one member function call.
 *
 * @tparam ListenerT Listener class that have some member function.
 *                   Only functions with void return value are supported.
 */
template<class ListenerT>
class ListenerContainer
{

// Public Mutators
public:


    /**
     * @brief Adds a listener into container.
     *
     * If the listener was stored  before, the new one is ignored. This
     * will secure all stored listener functions will be called only once
     * per listener.
     *
     * @param listener A pointer to listener that will be added.
     */
    void addListener(ListenerT* listener)
    {
        m_listeners.push_back(listener);
    }


    /**
     * @brief Remove listener from container.
     *
     * If pointer is not stored in this container nothing happened.
     *
     * @param listener A pointer to listener that should be removed.
     */
    void removeListener(ListenerT* listener)
    {
        //m_listeners.erase(listener);
    }


// Protected Operations
protected:


    /**
     * @brief Invoke a member function for all stored listeners.
     *
     * @tparam Args A list of member function arguments.
     *
     * @param function A pointer to listener's member function that will be
     * called for all stored listeners.
     */
    template<typename... Args>
    void invoke(void (ListenerT::*function)(Args...), Args&& ... args) const
    {
        // Foreach all listeners
        for (const auto& listener : m_listeners)
        {
            // Call member function with given arguments
            (listener->*function)(std::forward<Args>(args)...);
        }
    }


// Private Data Members
private:

    /// Stored listeners.
    DynamicArray<ListenerT*> m_listeners;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
