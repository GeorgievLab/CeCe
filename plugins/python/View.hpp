/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <utility>

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief RAII wrapper around Python API borrowed objects.
 */
template<typename T>
class View
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    View() = default;


    /**
     * @brief Constructor.
     *
     * @param ptr Object pointer.
     */
    View(T* ptr) noexcept
        : m_ptr(ptr)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Returns if handle is set.
     */
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Implicit cast to operator.
     */
    operator T*() const noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return Reference to object.
     */
    T& operator*() noexcept
    {
        return *m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return Reference to object.
     */
    const T& operator*() const noexcept
    {
        return *m_ptr;
    }


    /**
     * @brief Dereference operator.
     *
     * @return A pointer to object.
     */
    T* operator->() const noexcept
    {
        return m_ptr;
    }


// Public Accessors
public:


    /**
     * @brief Returns stored pointer.
     */
    T* get() const noexcept
    {
        return m_ptr;
    }


    /**
     * @brief Returns mutable stored pointer.
     */
    T*& getRef()& noexcept
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
 * @brief Create view from pointer.
 *
 * @param ptr Pointer.
 *
 * @return
 */
template<typename T>
View<T> makeView(T* ptr) noexcept
{
    return View<T>(ptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
