
#pragma once

/* ************************************************************************ */

// Boost
#include <boost/optional.hpp>

// Simulator
#include "core/compatibility.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "DrawableYeast.hpp"
#endif

// Module
#include "CellBase.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public CellBase
{

// Public Structures
public:


    /**
     * @brief Yeast bud.
     */
    struct Bud
    {
        /// Angle.
        units::Angle rotation = units::deg(0);

        /// Bud volume.
        units::Volume volume = units::um3(1);

#if ENABLE_PHYSICS
        /// Bud shape.
        b2CircleShape shape;
#else
        /// Position offset
        PositionVector offset;
#endif
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param type       Cell type.
     */
    explicit Yeast(simulator::Simulation& simulation,
                   simulator::Object::Type type = simulator::Object::Type::Dynamic) NOEXCEPT;


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
    bool hasBud() const NOEXCEPT
    {
        return m_bud.is_initialized();
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
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config,
                   simulator::Simulation& simulation) override;


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
     * @brief Render yeast.
     *
     * @param context
     */
    virtual void draw(render::Context& context) override;
#endif


// Protected Operations
protected:


#if ENABLE_PHYSICS
    /**
     * @brief Update yeast physics shape.
     */
    void updateShape();
#endif


// Private Data Members
private:

    /// Bud cell.
    boost::optional<Bud> m_bud;

#if ENABLE_RENDER
    render::ObjectPtr<DrawableYeast> m_renderObject;
#endif

#if ENABLE_PHYSICS
    /// Main cell shape.
    b2CircleShape m_shape;

    /// If shape must be updated.
    bool m_shapeForceUpdate = false;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
