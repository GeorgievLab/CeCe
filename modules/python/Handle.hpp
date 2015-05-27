
#pragma once

/* ************************************************************************ */

// C++
#include <utility>

// Python
#include <Python.h>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief RAII wrapper around Python API objects.
 */
template<typename T>
class Handle
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Handle() = default;


    /**
     * @brief Constructor.
     *
     * @param ptr Object pointer.
     */
    Handle(T* ptr) NOEXCEPT
        : m_ptr(ptr)
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     *
     * @param src Source handle.
     */
    Handle(const Handle& src) NOEXCEPT
        : m_ptr(src.m_ptr)
    {
        Py_INCREF(m_ptr);
    }


    /**
     * @brief Move constructor.
     *
     * @param src Source handle.
     */
    Handle(Handle&& src) NOEXCEPT
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
    Handle& operator=(T* ptr) NOEXCEPT
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
    Handle& operator=(const Handle& src) NOEXCEPT
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
    Handle& operator=(Handle&& src) NOEXCEPT
    {
        std::swap(m_ptr, src.m_ptr);
        return *this;
    }


    /**
     * @brief Returns if handle is set.
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


// Public Accessors
public:


    /**
     * @brief Returns stored pointer.
     */
    T* get() const NOEXCEPT
    {
        return m_ptr;
    }


    /**
     * @brief Returns mutable stored pointer.
     */
    T*& getRef()& NOEXCEPT
    {
        return m_ptr;
    }


// Private Data Members
private:


    /// Object pointer.
    T* m_ptr = nullptr;

};

/* ************************************************************************ */

/**
 * @brief Create handle from pointer.
 *
 * @param ptr Pointer.
 *
 * @return
 */
template<typename T>
Handle<T> makeHandle(T* ptr) NOEXCEPT
{
    return Handle<T>(ptr);
}

/* ************************************************************************ */

/**
 * @brief Create string handle from pointer.
 *
 * @param str String.
 *
 * @return
 */
inline Handle<PyObject> makeString(const char* str) NOEXCEPT
{
    return makeHandle(PyString_FromString(""));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
