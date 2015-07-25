/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <memory>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Shared smart pointer class.
 *
 * @tparam T Type of managed object.
 */
template<typename T>
using SharedPtr = std::shared_ptr<T>;

/* ************************************************************************ */

/**
 * @brief Weak smart pointer class.
 *
 * @tparam T Type of managed object.
 */
template<typename T>
using WeakPtr = std::weak_ptr<T>;

/* ************************************************************************ */

/**
 * @brief Make unique ptr function.
 *
 * @return
 */
template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
