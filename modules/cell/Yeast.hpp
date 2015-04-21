
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
 * @brief Predefined object flags.
 */
enum
{
    OBJECT_YEAST = 0x20
};

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
     */
    explicit Yeast(simulator::Simulation& simulation) noexcept;


#ifdef ENABLE_PHYSICS
    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param body
     */
    Yeast(simulator::Simulation& simulation, b2Body* body) noexcept;
#endif


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
        return m_bud.exists;
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
     * @brief Release bud cell.
     *
     * @return Created yeast cell.
     */
    Yeast* budRelease();


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
    struct
    {
        /// If yeast have bud.
        bool exists = false;

        /// Cell volume.
        units::Volume volume;

#ifdef ENABLE_PHYSICS
        b2Body* body = nullptr;
        b2CircleShape shape;
        b2DistanceJoint* joint;
#endif

    } m_bud;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
