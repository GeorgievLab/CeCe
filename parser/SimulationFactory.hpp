
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"
#include "core/UniquePtr.hpp"
#include "core/IStream.hpp"
#include "simulator/SimulationFactory.hpp"

/* ************************************************************************ */

namespace simulator { class Simulation; }

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

/**
 * @brief Parse simulation factory interface.
 */
class DLL_EXPORT SimulationFactory : public simulator::SimulationFactory
{


// Public Operations
public:


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    UniquePtr<simulator::Simulation> fromFile(const FilePath& filename) const override;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     * @param filename
     */
    UniquePtr<simulator::Simulation> fromSource(const String& source,
        const FilePath& filename = "<source>") const override;


// Protected Operations
protected:


    /**
     * @brief Parse simulation from input stream.
     *
     * @param source
     * @param filename
     *
     * @return
     */
    virtual UniquePtr<simulator::Simulation> fromStream(IStream& source,
        const String& filename) const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
