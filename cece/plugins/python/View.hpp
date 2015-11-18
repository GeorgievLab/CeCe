/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
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

// C++
#include <utility>

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief RAII wrapper around Python API borrowed objects.
 */
template<typename T>
class View
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    View() = default;


    /**
     * @brief Constructor.
     *
     * @param ptr Object pointer.
     */
    View(T* ptr) noexcept
        : m_ptr(ptr)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Returns if handle is set.
     */
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Implicit cast to operator.
     */
    operator T*() const noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return Reference to object.
     */
    T& operator*() noexcept
    {
        return *m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return Reference to object.
     */
    const T& operator*() const noexcept
    {
        return *m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return A pointer to object.
     */
    T* operator->() const noexcept
    {
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Returns stored pointer.
     */
    T* get() const noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Returns mutable stored pointer.
     */
    T*& getRef()& noexcept
    {
        return m_ptr;
    }


// Private Data Members
private:


    /// Object pointer.
    T* m_ptr = nullptr;

};

/* ************************************************************************ */

/// Implicit instantiation of this template (mostly used).
extern template class View<PyObject>;

/* ************************************************************************ */

/**
 * @brief Type alias for handle to PyObject.
 */
using ObjectView = View<PyObject>;

/* ************************************************************************ */

/**
 * @brief Create view from pointer.
 *
 * @param ptr Pointer.
 *
 * @return
 */
template<typename T>
View<T> makeView(T* ptr) noexcept
{
    return View<T>(ptr);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
