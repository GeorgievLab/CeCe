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
 * @brief Unique smart pointer class.
 *
 * @tparam T       Type of managed object.
 * @tparam Deleter Deleter object type.
 */
template<typename T, class Deleter = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, Deleter>;

/* ************************************************************************ */

/**
 * @brief Make unique ptr function.
 *
 * @return
 */
template<typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args)
{
    return UniquePtr<T>(new T{std::forward<Args>(args)...});
}

/* ************************************************************************ */

/**
 * @brief Make unique ptr function for custom ctor & dtor.
 *
 * @param value Managed value.
 * @param dtor  Destructor function.
 *
 * @return
 */
template<typename T, typename Dtor>
auto makeUniqueCustom(T* value, Dtor dtor) -> UniquePtr<T, Dtor>
{
    return UniquePtr<T, Dtor>(value, dtor);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
