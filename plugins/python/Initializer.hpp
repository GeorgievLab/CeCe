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
#include "core/Units.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Python.hpp"
#include "Source.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Simple wrapper functor for Python code.
 */
class Initializer
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Initializer();


    /**
     * @brief Destructor.
     */
    ~Initializer();


// Public Operations
public:


    /**
     * @brief Initialize from source.
     *
     * @param source Source code.
     */
    void initSource(const String& source)
    {
        m_source.initSource(source);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Initialize from file.
     *
     * @param filename Path to source file.
     */
    void initFile(const FilePath& filename)
    {
        m_source.initFile(filename);
        m_call = m_source.getFunction("call");
    }


    /**
     * @brief Call program.
     *
     * @param simulation Current simulation.
     */
    void operator()(simulator::Simulation& simulation) const;


// Private Data Members
private:


    /// Source.
    Source m_source;

    /// Call function.
    Handle<PyObject> m_call;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
