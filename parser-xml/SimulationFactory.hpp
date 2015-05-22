
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <istream>

// Simulator
#include "parser/SimulationFactory.hpp"

/* ************************************************************************ */

namespace parser {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML parser simulation factory.
 */
class SimulationFactory : public parser::SimulationFactory
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
    std::unique_ptr<simulator::Simulation> fromStream(std::istream& source,
        const std::string& filename) const override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
