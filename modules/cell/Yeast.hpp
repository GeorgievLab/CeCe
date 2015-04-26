
#pragma once

/* ************************************************************************ */

// C++
#include <memory>

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "DrawableYeast.hpp"
#endif

// Module
#include "CellBase.hpp"

/* ************************************************************************ */

namespace module {
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
                   simulator::Object::Type type = simulator::Object::Type::Dynamic) noexcept;


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


#ifdef ENABLE_RENDER
    /**
     * @brief Returns cell render object.
     *
     * @return
     */
    DrawableYeast& getRenderObject() noexcept
    {
        return m_renderObject;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns cell render object.
     *
     * @return
     */
    const DrawableYeast& getRenderObject() const noexcept
    {
        return m_renderObject;
    }
#endif


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


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize object for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


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
    std::unique_ptr<Bud> m_bud;

#if ENABLE_RENDER
    DrawableYeast m_renderObject;
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
