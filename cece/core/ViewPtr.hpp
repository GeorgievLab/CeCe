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
#include "cece/core/UniquePtr.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Wrapper around plain pointer. It have only semantic meaning.
 */
template<typename T>
class ViewPtr
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    ViewPtr() = default;


    /**
     * @brief Constructor.
     *
     * @param ptr Object pointer.
     */
    ViewPtr(T* ptr) noexcept
        : m_ptr(ptr)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param ptr Object pointer.
     */
    template<typename U>
    ViewPtr(const ViewPtr<U>& ptr) noexcept
        : m_ptr(dynamic_cast<T*>(ptr.get()))
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param ptr Unique pointer.
     */
    ViewPtr(const UniquePtr<T>& ptr) noexcept
        : m_ptr(ptr.get())
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param ptr Unique pointer.
     */
    template<typename U>
    ViewPtr(const UniquePtr<U>& ptr) noexcept
        : m_ptr(dynamic_cast<T*>(ptr.get()))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Returns if pointer is set.
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
     * @return Reference.
     */
    T& operator*() noexcept
    {
        return *m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return Reference.
     */
    const T& operator*() const noexcept
    {
        return *m_ptr;
    }


    /**
     * @brief Pointer access operator.
     *
     * @return Pointer.
     */
    T* operator->() noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Pointer access operator.
     *
     * @return Pointer.
     */
    const T* operator->() const noexcept
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


// Public Operations
public:


    /**
     * @brief Change viewed pointer.
     *
     * @param ptr New pointer.
     */
    void reset(T* ptr = nullptr) noexcept
    {
        m_ptr = ptr;
    }


    /**
     * @brief Release stored pointer.
     *
     * @return Previously stored pointer.
     */
    T* release() noexcept
    {
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }


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
ViewPtr<T> makeView(T* ptr) noexcept
{
    return ViewPtr<T>(ptr);
}

/* ************************************************************************ */

/**
 * @brief Compare view and unique pointer.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
template<typename T>
bool operator==(const ViewPtr<T>& lhs, const UniquePtr<T>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

/* ************************************************************************ */

/**
 * @brief Compare view and unique pointer.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
template<typename T>
bool operator==(const UniquePtr<T>& lhs, const ViewPtr<T>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

/* ************************************************************************ */

/**
 * @brief Compare view and unique pointer.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
template<typename T>
bool operator!=(const ViewPtr<T>& lhs, const UniquePtr<T>& rhs) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare view and unique pointer.
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
template<typename T>
bool operator!=(const UniquePtr<T>& lhs, const ViewPtr<T>& rhs) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
