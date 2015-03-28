
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Object.hpp"
#include "simulator/Position.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Cell generator.
 */
class CellGenerator : public Object
{

// Public Accessors
public:



// Public Mutators
public:



// Public Operations
public:


    /**
     * @brief Update object state.
     *
     * @param dt Time step.
     */
    void update(units::Duration dt) override;


// Private Data Members
private:

};

/* ************************************************************************ */

}

/* ************************************************************************ */
