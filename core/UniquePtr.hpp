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

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Unique smart pointer class.
 *
 * @tparam T Type of managed object.
 */
template<typename T>
using UniquePtr = std::unique_ptr<T>;

/* ************************************************************************ */

/**
 * @brief Make unique ptr function.
 *
 * @return
 */
template<typename T, typename... Args>
UniquePtr<T> makeUniquePtr(Args&&... args)
{
    return UniquePtr<T>(new T{std::forward<Args>(args)...});
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
