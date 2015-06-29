/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <utility>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

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
    View(T* ptr) NOEXCEPT
        : m_ptr(ptr)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Returns if handle is set.
     */
    explicit operator bool() const NOEXCEPT
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Implicit cast to operator.
     */
    operator T*() const NOEXCEPT
    {
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Returns stored pointer.
     */
    T* get() const NOEXCEPT
    {
        return m_ptr;
    }


    /**
     * @brief Returns mutable stored pointer.
     */
#ifndef _MSC_VER
    T*& getRef()& NOEXCEPT
    {
        return m_ptr;
    }
#endif


// Private Data Members
private:


    /// Object pointer.
    T* m_ptr = nullptr;

};

/* ************************************************************************ */

/**
 * @brief Create view from pointer.
 *
 * @param ptr Pointer.
 *
 * @return
 */
template<typename T>
View<T> makeView(T* ptr) NOEXCEPT
{
    return View<T>(ptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
