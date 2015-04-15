
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Predefined object flags.
 */
enum ObjectFlags
{
    OBJECT_STATIC = 0x01,
    OBJECT_DYNAMIC = 0x02
};

/* ************************************************************************ */

/**
 * @brief Basic simulation object.
 */
class Object
{

// Public Types
public:


    /// Object ID type.
    using IdType = unsigned long;

    /// Flags type.
    using FlagsType = unsigned long long;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Object();


    /**
     * @brief Destructor.
     */
    virtual ~Object();


// Public Accessors
public:


    /**
     * @brief Returns object ID.
     *
     * @return
     */
    IdType getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Returns object flags.
     *
     * @return
     */
    FlagsType getFlags() const noexcept
    {
        return m_flags;
    }


    /**
     * @brief Returns if object has given flag set.
     *
     * @param flag
     *
     * @return
     */
    bool hasFlag(FlagsType flag) const noexcept
    {
        return m_flags & flag;
    }


// Public Mutators
public:


    /**
     * @brief Set object flags.
     *
     * @param flags
     *
     * @note Use with caution.
     */
    void setFlags(FlagsType flags) noexcept
    {
        m_flags = flags;
    }


    /**
     * @brief Set object flag.
     *
     * @param flag to set
     *
     * @return
     */
    void setFlag(FlagsType flag) noexcept
    {
        m_flags |= flag;
    }


    /**
     * @brief Unset object flag.
     *
     * @param flag to set
     *
     * @return
     */
    void unsetFlag(FlagsType flag) noexcept
    {
        m_flags &= ~flag;
    }


// Public Operations
public:


    /**
     * @brief Cast object into required type.
     *
     * @return
     */
    template<typename T>
    T* cast() noexcept
    {
        assert(dynamic_cast<T*>(this));
        return static_cast<T*>(this);
    }


    /**
     * @brief Cast object into required type.
     *
     * @return
     */
    template<typename T>
    const T* cast() const noexcept
    {
        assert(dynamic_cast<const T*>(this));
        return static_cast<const T*>(this);
    }


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
    virtual void update(units::Duration dt) = 0;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize object for rendering.
     *
     * @param context
     */
    virtual void drawInit(render::Context& context)
    {
        // Nothing to do
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render object.
     *
     * @param context
     */
    virtual void draw(render::Context& context)
    {
        // Nothing to do
    }
#endif

// Private Data Members
private:

    /// ID generator.
    static IdType s_id;

    /// Object unique ID.
    IdType m_id;

    /// Objects flags.
    FlagsType m_flags = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
