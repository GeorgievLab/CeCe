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
#include <cassert>
#include <utility>

// CeCe
#include "cece/export.hpp"
#include "cece/render/Context.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Basic class for drawing objects.
 */
class CECE_EXPORT Object
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context
     */
    explicit Object(Context& context)
        : m_context(context)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    virtual ~Object()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Release object.
     */
    void release();


// Private Data Members
private:

    /// Rendering context.
    Context& m_context;

};

/* ************************************************************************ */

/**
 * @brief Wrapper for other types that can be managed as render objects.
 *
 * @tparam T
 */
template<typename T>
class ObjectWrapper : public Object, public T
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context
     * @param args
     */
    template<typename... Args>
    ObjectWrapper(Context& context, Args&&... args)
        : Object(context)
        , T(context, std::forward<Args>(args)...)
    {
        // Nothing to do
    }

};

/* ************************************************************************ */

/**
 * @brief Smart pointer like object wrapper.
 *
 * @tparam T
 */
template<typename T>
class ObjectPtr
{


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~ObjectPtr()
    {
        release();
    }


// Public Operators
public:


    /**
     * @brief Check if pointer is set.
     */
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Dereference pointer.
     *
     * @return
     */
    T& operator*() const noexcept
    {
        assert(m_ptr);
        return *m_ptr;
    }


    /**
     * @brief Return pointer access.
     *
     * @return
     */
    T* operator->() const noexcept
    {
        assert(m_ptr);
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Returns pointer.
     *
     * @return
     */
    T* get() const noexcept
    {
        return m_ptr;
    }


// Public Operations
public:


    /**
     * @brief Initialize object.
     *
     * @param context
     * @param args
     */
    template<typename... Args>
    void create(Context& context, Args&&... args)
    {
        // Create object
        m_ptr = context.createObject<ObjectWrapper<T>>(std::forward<Args>(args)...);
    }


    /**
     * @brief Release pointer.
     */
    void release()
    {
        if (m_ptr)
        {
            m_ptr->release();
            m_ptr = nullptr;
        }
    }


// Private Data Members
private:

    /// Pointer
    ObjectWrapper<T>* m_ptr = nullptr;

};

/* ************************************************************************ */

/**
 * @brief Smart pointer for render objects with reference counting.
 *
 * This pointer is usefull when same object is used in whole simulation
 * multiple times and is expensive to create new ones.
 *
 * @tparam T
 */
template<typename T>
class ObjectSharedPtr
{


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~ObjectSharedPtr()
    {
        release();
    }


// Public Operators
public:


    /**
     * @brief Check if pointer is set.
     */
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Dereference pointer.
     *
     * @return
     */
    T& operator*() const noexcept
    {
        assert(m_ptr);
        return *m_ptr;
    }


    /**
     * @brief Return pointer access.
     *
     * @return
     */
    T* operator->() const noexcept
    {
        assert(m_ptr);
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Returns pointer.
     *
     * @return
     */
    T* get() const noexcept
    {
        return m_ptr;
    }


// Public Operations
public:


    /**
     * @brief Create static object if is required and increase usage counter.
     *
     * @param context
     * @param args
     *
     * @return Object wrapper.
     */
    template<typename... Args>
    ObjectWrapper<T>* createStatic(Context& context, Args&&... args)
    {
        if (s_ptr)
        {
            ++s_count;
            return s_ptr;
        }

        // Create object
        assert(s_count == 0);
        s_ptr = context.createObject<ObjectWrapper<T>>(std::forward<Args>(args)...);
        s_count = 1;

        return s_ptr;
    }


    /**
     * @brief Initialize object.
     *
     * @param context
     * @param args
     */
    template<typename... Args>
    void create(Context& context, Args&&... args)
    {
        // Create static if is required
        m_ptr = createStatic(context, std::forward<Args>(args)...);
    }


    /**
     * @brief Release pointer.
     */
    void release()
    {
        if (m_ptr)
        {
            releaseStatic();
            m_ptr = nullptr;
        }
    }


    /**
     * @brief Release static pointer.
     */
    void releaseStatic()
    {
        // Release if counter is zero
        if (s_ptr && --s_count == 0)
        {
            s_ptr->release();
            s_ptr = nullptr;
            assert(s_count == 0);
        }
    }


// Private Data Members
private:

    /// Local pointer.
    ObjectWrapper<T>* m_ptr = nullptr;

    /// Static pointer.
    static ObjectWrapper<T>* s_ptr;

    /// Number of usages.
    /// TODO: thread sync??
    static unsigned int s_count;

};

/* ************************************************************************ */

template<typename T>
ObjectWrapper<T>* ObjectSharedPtr<T>::s_ptr = nullptr;

/* ************************************************************************ */

template<typename T>
unsigned int ObjectSharedPtr<T>::s_count = 0;

/* ************************************************************************ */

}
}

/* ************************************************************************ */
