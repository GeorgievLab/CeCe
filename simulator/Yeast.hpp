
#pragma once

/* ************************************************************************ */

// C++
#include <memory>

// Physics
#include "simulator/Cell.h"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.h"
#endif

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


    /**
     * @brief Returns yeast's bud.
     *
     * @return
     */
    Yeast* getBud() const noexcept
    {
        return m_bud.get();
    }


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


    /**
     * @brief Create new yeast bud.
     *
     * @param volume Bud volume.
     */
    void budCreate(Volume volume);


    /**
     * @brief Release bud cell.
     */
    void budRelease();


#ifdef ENABLE_RENDER

    /**
     * @brief Render cell.
     *
     * @param context
     */
    virtual void draw(render::Context& context);

#endif

// Private Data Members
private:

    /// Bud cell.
    std::unique_ptr<Yeast> m_bud;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
