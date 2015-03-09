
#pragma once

/* ************************************************************************ */

// C++
#include <cstddef>
#include <memory>
#include <functional>

// Core
#include "core/Units.hpp"

// Physics
#include "physics/Cell.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell representation.
 */
class Cell : public physics::Cell
{

// Public Types
public:


    /// Cell ID type.
    using Id = unsigned long;

    /// Behaviour function type.
    using BehaviourFn = std::function<void(Cell&)>;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param shape Cell shape.
     */
    Cell(World* world, Shape shape = Shape::Sphere);


    /**
     * @brief Destructor.
     */
    virtual ~Cell();


// Public Accessors
public:


    /**
     * @brief Returns cell ID.
     *
     * @return
     */
    Id getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Returns world.
     *
     * @return
     */
    World* getWorld() const noexcept
    {
        return m_world;
    }


    /**
     * @brief Returns cell behaviour function.
     *
     * @return
     */
    const BehaviourFn& getBehaviour() const noexcept
    {
        return m_behaviour;
    }


    /**
     * @brief Returns a number of GFP proteins.
     *
     * @return
     */
    unsigned int getGfp() const noexcept
    {
        return m_gfp;
    }


    /**
     * @brief Returns a number of RFP proteins.
     *
     * @return
     */
    unsigned int getRfp() const noexcept
    {
        return m_rfp;
    }


    /**
     * @brief Returns a number of YFP proteins.
     *
     * @return
     */
    unsigned int getYfp() const noexcept
    {
        return m_yfp;
    }


// Public Mutators
public:


    /**
     * @brief Set cell behaviour function.
     *
     * @param fn Function.
     */
    void setBehaviour(BehaviourFn fn) noexcept
    {
        m_behaviour = std::move(fn);
    }


    /**
     * @brief Set a number of GFP proteins in the cell.
     *
     * @param gfp
     */
    void setGfp(unsigned int gfp) noexcept
    {
        m_gfp = gfp;
    }


    /**
     * @brief Set a number of RFP proteins in the cell.
     *
     * @param rfp
     */
    void setRfp(unsigned int rfp) noexcept
    {
        m_rfp = rfp;
    }


    /**
     * @brief Set a number of YFP proteins in the cell.
     *
     * @param yfp
     */
    void setYfp(unsigned int yfp) noexcept
    {
        m_yfp = yfp;
    }


// Public Operations
public:


    /**
     * @brief Update cell state.
     */
    virtual void update();

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

    /// ID generator.
    static Id s_id;

    /// Cell ID
    Id m_id;

    /// Cell world.
    World* const m_world;

    /// Behaviour function.
    BehaviourFn m_behaviour;

    /// Number of GFP proteins.
    unsigned int m_gfp = 0;

    /// Number of RFP proteins.
    unsigned int m_rfp = 0;

    /// Number of YFP proteins.
    unsigned int m_yfp = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

