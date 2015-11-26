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

// CeCe
#include "cece/core/UniquePtr.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Factory interface template.
 *
 * @tparam T    Type of constructed object.
 * @tparam Args Constructor arguments.
 */
template<typename T, typename... Args>
class Factory
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Factory()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Create an object.
     *
     * @param args Constructor arguments.
     *
     * @return Created object pointer.
     */
    virtual UniquePtr<T> create(Args... args) const noexcept = 0;

};

/* ************************************************************************ */

/**
 * @brief Factory for specific type.
 *
 * @tparam ParentFactory
 * @tparam T
 * @tparam Base
 * @tparam Args
 */
template<template<typename, typename...> class ParentFactory, typename T, typename Base, typename... Args>
class FactoryTyped : public ParentFactory<Base, Args...>
{

// Public Operations
public:


    /**
     * @brief Create an object.
     *
     * @param args Constructor arguments.
     *
     * @return Created object pointer.
     */
    UniquePtr<Base> create(Args... args) const noexcept override
    {
        return makeUnique<T>(std::forward<Args>(args)...);
    }

};

/* ************************************************************************ */

/**
 * @brief Factory with callable backend.
 *
 * @tparam ParentFactory
 * @tparam Callable
 * @tparam Base
 * @tparam Args
 */
template<template<typename, typename...> class ParentFactory, typename Callable, typename Base, typename... Args>
class FactoryCallable : public ParentFactory<Base, Args...>
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param callable Callable object (functor, function, lambda).
     */
    explicit FactoryCallable(Callable callable) noexcept
        : m_callable(std::move(callable))
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Create an object.
     *
     * @param args Constructor arguments.
     *
     * @return Created object pointer.
     */
    UniquePtr<Base> create(Args... args) const noexcept override
    {
        return m_callable(std::forward<Args>(args)...);
    }


// Private Data Members
private:

    /// Callable object.
    Callable m_callable;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
