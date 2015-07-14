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

// Plugin
#include "Python.hpp"

// C++
#include <utility>

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief RAII wrapper around Python API objects.
 *
 * @tparam T Type of managed object, mostly PyObject.
 */
template<typename T>
class Handle
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Handle() noexcept
        : m_ptr(nullptr)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param ptr Object pointer.
     */
    Handle(T* ptr) noexcept
        : m_ptr(ptr)
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     *
     * @param src Source handle.
     */
    Handle(const Handle& src) noexcept
        : m_ptr(src.m_ptr)
    {
        Py_XINCREF(m_ptr);
    }


    /**
     * @brief Move constructor.
     *
     * @param src Source handle.
     */
    Handle(Handle&& src) noexcept
        : m_ptr(src.m_ptr)
    {
        src.m_ptr = nullptr;
    }


    /**
     * @brief Destructor.
     */
    ~Handle()
    {
        Py_XDECREF(m_ptr);
    }


// Public Operators
public:


    /**
     * @brief Assignment operator.
     *
     * @param ptr Object pointer.
     *
     * @return *this
     */
    Handle& operator=(T* ptr) noexcept
    {
        m_ptr = ptr;
        return *this;
    }


    /**
     * @brief Copy assignment operator.
     *
     * @param src Source handle.
     *
     * @return *this
     */
    Handle& operator=(const Handle& src) noexcept
    {
        m_ptr = src.m_ptr;
        Py_XINCREF(m_ptr);
        return *this;
    }


    /**
     * @brief Move assignment operator.
     *
     * @param src Source handle.
     */
    Handle& operator=(Handle&& src) noexcept
    {
        std::swap(m_ptr, src.m_ptr);
        return *this;
    }


    /**
     * @brief Returns if handle is set.
     */
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Implicit cast to value operator.
     */
    operator T*() const noexcept
    {
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Returns stored pointer.
     *
     * @return Stored pointer.
     */
    T* get() const noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Returns mutable stored pointer.
     *
     * @return
     */
    T*& getRef()& noexcept
    {
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Release stored pointer.
     *
     * @return Pointer.
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
    T* m_ptr;

};

/* ************************************************************************ */

/**
 * @brief Create handle from pointer. It doesn't increase object counter.
 *
 * @param ptr Pointer.
 *
 * @return Handle object.
 */
template<typename T>
Handle<T> makeHandle(T* ptr) noexcept
{
    return Handle<T>(ptr);
}

/* ************************************************************************ */

/// Implicit instantiation of this template (mostly used).
extern template class Handle<PyObject>;

/* ************************************************************************ */

/**
 * @brief Type alias for handle to PyObject.
 */
using ObjectHandle = Handle<PyObject>;

/* ************************************************************************ */

}
}

/* ************************************************************************ */
