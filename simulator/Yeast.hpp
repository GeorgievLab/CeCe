
#pragma once

/* ************************************************************************ */

// C++
#include <memory>

// Physics
#include "simulator/Cell.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public Cell
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     */
    Yeast(World* world);


    /**
     * @brief Destructor.
     */
    virtual ~Yeast();


// Public Accessors
public:


    /**
     * @brief Get cell volume.
     *
     * @return
     */
    Volume getVolume() const noexcept override;


// Public Mutators
public:


    /**
     * @brief Set cell volume.
     *
     * @param volume
     */
    void setVolume(Volume volume) noexcept override;


// Public Operations
public:


    /**
     * @brief Update yeast state.
     */
    void update() override;


// Private Data Members
private:

    /// Bud cell.
    std::unique_ptr<Yeast> m_bud;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
