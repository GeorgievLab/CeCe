
#pragma once

/* ************************************************************************ */

// C++
#include <vector>
#include <memory>
#include <cassert>
#include <type_traits>

// Simulator
#include "simulator/Velocity.hpp"
#include "simulator/Object.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#include "render/Grid.hpp"
#include "render/GridVector.hpp"
#include "render/GridValue.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Type for step number.
 */
using StepNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief World for cells.
 */
class World
{

// Public Enums
public:

#ifdef ENABLE_RENDER

    enum RenderFlags
    {
        RENDER_NONE = 0,
        RENDER_GRID = 1,
        RENDER_VELOCITY = 2,
    };

    using RenderFlagsType = typename std::underlying_type<RenderFlags>::type;

#endif

// Public Types
public:


    /// Grid for storing velocities.
    using GridVelocity = Grid<Velocity>;

    /// Grid for storing signals.
    using GridSignal = Grid<float>;


// Public Types
public:


    /// Object container type.
    using ObjectContainer = std::vector<std::unique_ptr<Object>>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    World() noexcept;


    /**
     * @brief Virtual destructor.
     */
    virtual ~World();


// Public Accessors
public:


    /**
     * @brief Returns step number.
     *
     * @return
     */
    StepNumber getStepNumber() const noexcept
    {
        return m_stepNumber;
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    const ObjectContainer& getObjects() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns world size.
     *
     * @return
     */
    const Vector<units::Length>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Returns world width.
     *
     * @return
     */
    units::Length getWidth() const noexcept
    {
        return getSize().getWidth();
    }


    /**
     * @brief Returns world height.
     *
     * @return
     */
    units::Length getHeight() const noexcept
    {
        return getSize().getHeight();
    }


    /**
     * @brief Calculate start position of the world.
     *
     * @return
     */
    Vector<units::Length> getStartPosition() const noexcept
    {
        return getSize() * -0.5f;
    }



// Public Mutators
public:


    /**
     * @brief Resize world.
     *
     * @param size
     */
    void resize(Vector<units::Length> size) noexcept
    {
        m_size = std::move(size);
    }


    /**
     * @brief Resize world.
     *
     * @param width
     * @param height
     */
    void resize(units::Length width, units::Length height) noexcept
    {
        resize({width, height});
    }


    /**
     * @brief Add a new object to the world.
     *
     * @param obj
     */
    template<typename T>
    T* addObject(std::unique_ptr<T> obj)
    {
        assert(obj);
        m_objects.push_back(std::move(obj));
        T* ptr = static_cast<T*>(m_objects.back().get());
#ifdef ENABLE_RENDER
        m_renderInitList.push_back(ptr);
#endif
        return ptr;
    }


    /**
     * @brief Create a new object.
     *
     * @param args...
     *
     * @return
     */
    template<typename T, typename... Args>
    T* createObject(Args&&... args)
    {
        return addObject(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
    }


// Public Operations
public:


    /**
     * @brief Clean world data.
     */
    void clean();


    /**
     * @brief Reset world.
     */
    void reset();


    /**
     * @brief Update world.
     *
     * @param dt
     */
    void update(units::Duration dt) noexcept;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize world for rendering.
     *
     * @param context
     */
    void renderInit(render::Context& context);
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render world.
     *
     * @param context
     * @param flags
     */
    void render(render::Context& context, RenderFlagsType flags = RENDER_NONE);
#endif


    /**
     * @brief Calculate world step by given grid sizes.
     *
     * @param size Grid size.
     *
     * @return
     */
    template<typename T>
    Vector<units::Length> calcStep(const Vector<T>& size) const noexcept
    {
        return getSize() / size;
    }


// Private Data Members
private:

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// World size.
    Vector<units::Length> m_size{units::um(400), units::um(400)};

    /// World objects.
    ObjectContainer m_objects;

#ifdef ENABLE_RENDER
    /// Render grid
    std::unique_ptr<render::Grid> m_renderGrid;

    /// List of objects that requires init.
    std::vector<Object*> m_renderInitList;
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */
