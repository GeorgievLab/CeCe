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

// C++
#include <utility>
#include <type_traits>

// CeCe
#include "cece/export.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

/**
 * @brief Define pointer container specialization.
 */
#define CECE_PTR_CONTAINER(...) \
    namespace cece { inline namespace core { template class CECE_EXPORT PtrContainer<__VA_ARGS__>; } }

/**
 * @brief Define extern pointer container specialization.
 */
#define CECE_PTR_CONTAINER_EXTERN(...) \
    namespace cece { inline namespace core { extern template class CECE_EXPORT PtrContainer<__VA_ARGS__>; } }

/**
 * @brief Define pointer container specialization.
 */
#define CECE_PTR_CONTAINER_INST(...) \
    namespace cece { inline namespace core { template class PtrContainer<__VA_ARGS__>; } }

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Container for pointers.
 */
template<typename T>
class PtrContainer
{

// Public Types
public:


    /// Pointer type.
    using type = T;


// Public Operators
public:


    /**
     * @brief Returns n-th object.
     *
     * @param pos Required position.
     *
     * @return Pointer to object.
     *
     * @warning No boundary checking.
     */
    ViewPtr<T> operator[](std::size_t pos) const noexcept
    {
        return m_data[pos];
    }


// Public Accessors
public:


    /**
     * @brief Returns a number of stored objects.
     *
     * @return
     */
    std::size_t getCount() const noexcept
    {
        return m_data.size();
    }


    /**
     * @brief Returns n-th object.
     *
     * @param pos Required position.
     *
     * @return Pointer to object.
     */
    ViewPtr<T> get(std::size_t pos) const
    {
        return m_data.at(pos);
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    typename DynamicArray<UniquePtr<T>>::const_iterator begin() const noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    typename DynamicArray<UniquePtr<T>>::const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    typename DynamicArray<UniquePtr<T>>::const_iterator end() const noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    typename DynamicArray<UniquePtr<T>>::const_iterator cend() const noexcept
    {
        return m_data.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store an object.
     *
     * @param object The object to store.
     *
     * @return View pointer to stored object.
     */
    ViewPtr<T> add(UniquePtr<T> object)
    {
        m_data.push_back(std::move(object));
        return m_data.back();
    }


    /**
     * @brief Create and store an object.
     *
     * @tparam Args Construction argument types.
     *
     * @param args Construction arguments.
     *
     * @return View pointer to stored object.
     */
    template<typename... Args>
    ViewPtr<T> create(Args&&... args)
    {
        return add(makeUnique<T>(std::forward<Args>(args)...));
    }


    /**
     * @brief Create and store an object.
     *
     * @tparam T2   Type of constructed object.
     * @tparam Args Construction argument types.
     *
     * @param args Construction arguments.
     *
     * @return View pointer to stored object.
     */
    template<typename T2, typename... Args>
    ViewPtr<T2> create(Args&&... args)
    {
        return add(makeUnique<T2>(std::forward<Args>(args)...));
    }


    /**
     * @brief Remove object from container.
     *
     * @param object The object to remove.
     *
     * @return A pointer to removed object.
     */
    UniquePtr<T> remove(ViewPtr<T> object)
    {
        for (auto itB = m_data.begin(), itE = m_data.end(); itB != itE; ++itB)
        {
            if (itB->get() == object.get())
            {
                auto ptr = std::move(*itB);
                m_data.erase(itB);
                return ptr;
            }
        }

        // Not found
        return nullptr;
    }


    /**
     * @brief Clear container.
     */
    void clear()
    {
        m_data.clear();
    }


// Protected Operations
protected:


    /**
     * @brief Invoke a member function for all stored objects.
     *
     * @tparam Args A list of member function arguments.
     *
     * @param function A pointer to object's member function that will be
     *                 called for all stored listeners.
     * @param args...
     */
    template<typename Fn, typename... Args1, typename... Args2>
    void invoke(Fn fn, Args2&&... args) const
    {
        static_assert(std::is_member_function_pointer<Fn>::value, "Fn is not a member function.");

        // Foreach all object
        for (const auto& object : m_data)
        {
            // Call member function with given arguments
            (object.get()->*fn)(std::forward<Args2>(args)...);
        }
    }


// Private Data Members
private:

    /// Stored pointers.
    DynamicArray<UniquePtr<T>> m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
