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
#include <cassert>
#include <utility>

// Simulator
#include "core/compatibility.hpp"
#include "render/Context.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Basic class for drawing objects.
 */
class DLL_EXPORT Object
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
    explicit operator bool() const NOEXCEPT
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Dereference pointer.
     *
     * @return
     */
    T& operator*() const NOEXCEPT
    {
        assert(m_ptr);
        return *m_ptr;
    }


    /**
     * @brief Return pointer access.
     *
     * @return
     */
    T* operator->() const NOEXCEPT
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
    T* get() const NOEXCEPT
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
        if (m_ptr) m_ptr->release();
    }


// Private Data Members
private:

    /// Pointer
    ObjectWrapper<T>* m_ptr = nullptr;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
