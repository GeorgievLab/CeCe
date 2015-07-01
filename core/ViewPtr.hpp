/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

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
    ViewPtr(T* ptr) NOEXCEPT
        : m_ptr(ptr)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param ptr Unique pointer.
     */
    ViewPtr(const UniquePtr<T>& ptr) NOEXCEPT
        : m_ptr(ptr.get())
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Returns if pointer is set.
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


    /**
     * @brief Dereference operator.
     *
     * @return Reference.
     */
    T& operator*() NOEXCEPT
    {
        return *m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return Reference.
     */
    const T& operator*() const NOEXCEPT
    {
        return *m_ptr;
    }


    /**
     * @brief Pointer access operator.
     *
     * @return Pointer.
     */
    T* operator->() NOEXCEPT
    {
        return m_ptr;
    }


    /**
     * @brief Pointer access operator.
     *
     * @return Pointer.
     */
    const T* operator->() const NOEXCEPT
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


// Public Operations
public:


    /**
     * @brief Change viewed pointer.
     *
     * @param ptr New pointer.
     */
    void reset(T* ptr = nullptr) NOEXCEPT
    {
        m_ptr = ptr;
    }


    /**
     * @brief Release stored pointer.
     *
     * @return Previously stored pointer.
     */
    T* release() NOEXCEPT
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
ViewPtr<T> makeView(T* ptr) NOEXCEPT
{
    return ViewPtr<T>(ptr);
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
