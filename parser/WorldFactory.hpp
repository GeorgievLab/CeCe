
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

// Simulator
#include "simulator/WorldFactory.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

/**
 * @brief WorldLoader interface.
 */
class WorldFactory : public simulator::WorldFactory
{

// Public Operations
public:


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    std::unique_ptr<simulator::World> fromFile(const std::string& filename) const override;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    std::unique_ptr<simulator::World> fromSource(const std::string& source) const override;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
