
#pragma once

/* ************************************************************************ */

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

// Module
#include "Cell.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public Cell
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     *
     * @param simulation
     * @param parent     Parent yeast.
     */
    explicit Yeast(simulator::Simulation& simulation, Yeast* parent = nullptr) noexcept;


    /**
     * @brief Destructor.
     */
    ~Yeast();


// Public Accessors
public:


    /**
     * @brief Returns if yeast has bud.
     *
     * @return
     */
    bool hasBud() const noexcept
    {
        return m_bud != nullptr;
    }


    /**
     * @brief Returns if yeast is bud.
     *
     * @return
     */
    bool isBud() const noexcept
    {
        return m_parent != nullptr;
    }


// Public Operations
public:


    /**
     * @brief Update yeast state.
     *
     * @param dt Time step.
     */
    void update(units::Duration dt) override;


    /**
     * @brief Create new yeast bud.
     */
    void budCreate();


    /**
     * @brief Release bud yeast.
     */
    void budRelease();


#if ENABLE_RENDER
    /**
     * @brief Render cell.
     *
     * @param context
     */
    virtual void draw(render::Context& context);
#endif

// Private Data Members
private:

    /// Parent yeast.
    Yeast* m_parent;

    /// Bud cell.
    Yeast* m_bud = nullptr;

#if ENABLE_PHYSICS
    // Bud joint
    b2DistanceJoint* m_joint;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
