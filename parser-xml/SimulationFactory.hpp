/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"
#include "core/IStream.hpp"
#include "core/FilePath.hpp"
#include "parser/SimulationFactory.hpp"

/* ************************************************************************ */

namespace simulator { class Simulation; }

/* ************************************************************************ */

namespace parser {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML parser simulation factory.
 */
class DLL_EXPORT SimulationFactory : public parser::SimulationFactory
{


// Protected Operations
protected:


    /**
     * @brief Parse simulation from stream.
     *
     * @param simulator
     * @param source
     * @param filename
     *
     * @return
     */
    UniquePtr<simulator::Simulation> fromStream(IStream& source,
        const FilePath& filename) const override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
