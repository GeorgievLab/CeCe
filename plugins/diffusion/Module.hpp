/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <algorithm>

// Simulator
#include "core/Real.hpp"
#include "core/String.hpp"
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "core/Range.hpp"
#include "simulator/Module.hpp"

#if ENABLE_RENDER
#  include "render/Color.hpp"
#  include "render/Context.hpp"
#  include "render/Object.hpp"
#  if CONFIG_PLUGIN_diffusion_SMOOTH
#    include "render/GridColorSmooth.hpp"
#  else
#    include "render/GridColor.hpp"
#  endif
#endif

#if THREAD_SAFE
#  include "core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion.
 */
class Module : public simulator::Module
{

// Public Types
public:


    /// Signal value type.
    using SignalConcentration = units::MolarConcentration;

    /// Signal identificator.
    using SignalId = unsigned int;

    /// Grid type.
    using GridType = Grid<SignalConcentration>;

    /// Grid type.
    using Coordinate = typename GridType::CoordinateType;

    /// Grid size type
    using SizeType = Vector<typename GridType::SizeType>;

    /// Diffusion rate (m^2/s).
    using DiffusionRate = units::Divide<units::Area, units::Time>::type;

    /// Degradation rate (%/s).
    using DegradationRate = units::Inverse<units::Time>::type;


// Public Constants
public:


    /**
     * This value is used to diffuse grid cell into OFFSET cell around.
     * It also extend inner grid size by 2 * OFFSET so then the boundary
     * checking is not need.
     */
    static constexpr typename Coordinate::value_type OFFSET = 1;

    /// Invalid signal ID.
    static constexpr SignalId INVALID_SIGNAL_ID = -1;


// Public Ctors & Dtors
public:


#if ENABLE_RENDER
    /**
     * @brief Constructor.
     */
    Module() noexcept
        : simulator::Module(100)
    {
        // Nothing to do
    }
#endif


// Public Accessors
public:


    /**
     * @brief Returns signal grid size.
     *
     * @return Current size.
     */
    const SizeType& getGridSize() const noexcept
    {
        return m_gridSize;
    }


    /**
     * @brief Returns number of signals.
     *
     * @return Number of signals.
     */
    size_t getSignalCount() const noexcept
    {
        return m_gridsFront.size();
    }


    /**
     * @brief Returns range of signal IDs.
     *
     * @return
     */
    IteratorRange<ValueIterator<SignalId>> getSignalIds() const noexcept
    {
        return range<SignalId>(0u, getSignalCount());
    }


    /**
     * @brief Returns signal identifier.
     *
     * @param name Signal name.
     *
     * @return Signal identifier or INVALID_SIGNAL_ID
     */
    SignalId getSignalId(const String& name) const noexcept
    {
        auto it = std::find(m_names.begin(), m_names.end(), name);
        return it != m_names.end() ? std::distance(m_names.begin(), it) : INVALID_SIGNAL_ID;
    }


    /**
     * @brief Returns signal name.
     *
     * @param id Signal identifier.
     *
     * @return Signal name or empty string.
     */
    const String& getSignalName(SignalId id) const noexcept
    {
        static const String empty;
        return (id < m_names.size()) ? m_names[id] : empty;
    }


    /**
     * @brief Returns mutable signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     *
     * @return
     */
    SignalConcentration& getSignalFront(SignalId id, const Coordinate& coord) noexcept
    {
        return m_gridsFront[id][coord + OFFSET];
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignalFront(SignalId id, const Coordinate& coord) const noexcept
    {
        return m_gridsFront[id][coord + OFFSET];
    }


    /**
     * @brief Returns mutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    SignalConcentration& getSignalFront(const String& name, const Coordinate& coord)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getSignalFront(id, coord);
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignalFront(const String& name, const Coordinate& coord) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getSignalFront(id, coord);
    }


    /**
     * @brief Returns mutable signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     *
     * @return
     */
    SignalConcentration& getSignalBack(SignalId id, const Coordinate& coord) noexcept
    {
        return m_gridsBack[id][coord + OFFSET];
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignalBack(SignalId id, const Coordinate& coord) const noexcept
    {
        return m_gridsBack[id][coord + OFFSET];
    }


    /**
     * @brief Returns mutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    SignalConcentration& getSignalBack(const String& name, const Coordinate& coord)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getSignalBack(id, coord);
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignalBack(const String& name, const Coordinate& coord) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getSignalBack(id, coord);
    }


    /**
     * @brief Returns mutable signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     *
     * @return
     */
    SignalConcentration& getSignal(SignalId id, const Coordinate& coord) noexcept
    {
        return getSignalFront(id, coord);
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignal(SignalId id, const Coordinate& coord) const noexcept
    {
        return getSignalFront(id, coord);
    }


    /**
     * @brief Returns mutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    SignalConcentration& getSignal(const String& name, const Coordinate& coord)
    {
        return getSignalFront(name, coord);
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignal(const String& name, const Coordinate& coord) const
    {
        return getSignalFront(name, coord);
    }


    /**
     * @brief Returns signal diffussion rate.
     *
     * @param id Signal identifier.
     *
     * @return
     */
    DiffusionRate getDiffusionRate(SignalId id) const noexcept
    {
        return m_diffusionRates[id];
    }


    /**
     * @brief Returns signal diffussion rate.
     *
     * @param name Signal name.
     *
     * @return
     */
    DiffusionRate getDiffusionRate(const String& name) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getDiffusionRate(id);
    }


    /**
     * @brief Returns signal degradation rate.
     *
     * @param id Signal identifier.
     *
     * @return
     */
    DegradationRate getDegradationRate(SignalId id) const noexcept
    {
        return m_degradationRates[id];
    }


    /**
     * @brief Returns signal degradation rate.
     *
     * @param name Signal name.
     *
     * @return
     */
    DegradationRate getDegradationRate(const String& name) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getDegradationRate(id);
    }


#if ENABLE_RENDER
    /**
     * @brief Get signal color.
     *
     * @param id Signal identifier.
     *
     * @return Signal color.
     */
    const render::Color& getSignalColor(SignalId id) const noexcept
    {
        return m_colors[id];
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Get signal color.
     *
     * @param name Signal name.
     *
     * @return Signal color.
     */
    const render::Color& getSignalColor(const String& name) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getSignalColor(id);
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Get signal saturation (only for rendering).
     *
     * @param id Signal identifier.
     *
     * @return
     */
    SignalConcentration getSignalSaturation(SignalId id) const noexcept
    {
        return m_signalSaturation[id];
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Get signal saturation (only for rendering).
     *
     * @param name Signal name.
     *
     * @return
     */
    SignalConcentration getSignalSaturation(const String& name) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return getSignalSaturation(id);
    }
#endif


    /**
     * @brief Check if coordinates are in range.
     *
     * @param coord Tested coordinates.
     *
     * @return
     */
    bool inRange(const Coordinate& coord) const noexcept
    {
        return coord.inRange(Coordinate{0}, m_gridSize);
    }


#if THREAD_SAFE
    /**
     * @brief Returns module mutex.
     *
     * @return
     */
    Mutex& getMutex() noexcept
    {
        return m_mutex;
    }
#endif


    /**
     * @brief Check if there is an obstacle at given coordinates.
     *
     * @param coord
     *
     * @return
     */
    bool isObstacle(const Coordinate& coord) const noexcept
    {
        return m_obstacles[coord + OFFSET];
    }


// Public Mutators
public:


    /**
     * @brief Resize signal grids.
     *
     * @param size New grid size.
     */
    void setGridSize(SizeType size);


    /**
     * @brief Register new signal.
     *
     * @param name    Signal name.
     * @param rate    Diffusion rate.
     * @param degRate Degradation rate.
     *
     * @return Signal identifier.
     */
    SignalId registerSignal(String name, DiffusionRate rate, DegradationRate degRate = {});


    /**
     * @brief Set signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     * @param value Signal value.
     *
     * @return
     */
    void setSignalFront(SignalId id, const Coordinate& coord, SignalConcentration value) noexcept
    {
        m_gridsFront[id][coord + OFFSET] = value;
    }


    /**
     * @brief Set signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     * @param value Signal value.
     *
     * @return
     */
    void setSignalFront(const String& name, const Coordinate& coord, SignalConcentration value)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        m_gridsFront[id][coord + OFFSET] = value;
    }


    /**
     * @brief Set signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     * @param value Signal value.
     *
     * @return
     */
    void setSignalBack(SignalId id, const Coordinate& coord, SignalConcentration value) noexcept
    {
        m_gridsBack[id][coord + OFFSET] = value;
    }


    /**
     * @brief Set signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     * @param value Signal value.
     *
     * @return
     */
    void setSignalBack(const String& name, const Coordinate& coord, SignalConcentration value)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        m_gridsBack[id][coord + OFFSET] = value;
    }


    /**
     * @brief Set signal value at given coordinates.
     *
     * @param id    Signal identifier.
     * @param coord Coordinates.
     * @param value Signal value.
     *
     * @return
     */
    void setSignal(SignalId id, const Coordinate& coord, SignalConcentration value) noexcept
    {
        setSignalFront(id, coord, value);
    }


    /**
     * @brief Set signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     * @param value Signal value.
     *
     * @return
     */
    void setSignal(const String& name, const Coordinate& coord, SignalConcentration value)
    {
        setSignalFront(name, coord, value);
    }


    /**
     * @brief Set signal diffussion rate.
     *
     * @param id   Signal identifier.
     * @param rate Diffusion rate.
     */
    void setDiffusionRate(SignalId id, DiffusionRate rate) noexcept
    {
        m_diffusionRates[id] = rate;
    }


    /**
     * @brief Set signal diffussion rate.
     *
     * @param name Signal name.
     * @param rate Diffusion rate.
     */
    void setDiffusionRate(const String& name, DiffusionRate rate)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        setDiffusionRate(id, rate);
    }


    /**
     * @brief Set signal degradation rate.
     *
     * @param id   Signal identifier.
     * @param rate Degradation rate.
     */
    void setDegradationRate(SignalId id, DegradationRate rate) noexcept
    {
        m_degradationRates[id] = rate;
    }


    /**
     * @brief Set signal degradation rate.
     *
     * @param name Signal name.
     * @param rate Degradation rate.
     */
    void setDegradationRate(const String& name, DegradationRate rate)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        setDegradationRate(id, rate);
    }


#if ENABLE_RENDER
    /**
     * @brief Set signal color.
     *
     * @param id    Signal identifier.
     * @param color New signal color.
     */
    void setSignalColor(SignalId id, render::Color color) noexcept
    {
        m_colors[id] = color;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Set signal color.
     *
     * @param name  Signal name.
     * @param color New signal color.
     */
    void setSignalColor(const String& name, render::Color color)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        setSignalColor(id, color);
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Change signal saturation.
     *
     * @param id         Signal identifier.
     * @param saturation Signal value when color is max.
     */
    void setSignalSaturation(SignalId id, SignalConcentration saturation) noexcept
    {
        m_signalSaturation[id] = saturation;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Change signal saturation.
     *
     * @param name       Signal name.
     * @param saturation Signal value when color is max.
     */
    void setSignalSaturation(const String& name, SignalConcentration saturation)
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        setSignalSaturation(id, saturation);
    }
#endif


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param world World object.
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;


    /**
     * @brief Update drawable.
     */
    void updateDrawable() const;
#endif


    /**
     * @brief Swap signal grids.
     *
     * @param id Signal identifier.
     */
    void swap(SignalId id) noexcept
    {
        std::swap(m_gridsFront[id], m_gridsBack[id]);
    }


    /**
     * @brief Swap all signal grids.
     */
    void swapAll() noexcept
    {
        // Swap grids
        for (auto id : getSignalIds())
            swap(id);
    }


    /**
     * @brief Clear front grid.
     *
     * @param id Signal identifier.
     */
    void clearFront(SignalId id) noexcept
    {
        std::fill(m_gridsFront[id].begin(), m_gridsFront[id].end(), SignalConcentration{});
    }


    /**
     * @brief Clear back grid.
     *
     * @param id Signal identifier.
     */
    void clearBack(SignalId id) noexcept
    {
        std::fill(m_gridsBack[id].begin(), m_gridsBack[id].end(), SignalConcentration{});
    }


// Protected Operations
protected:


    /**
     * @brief Update signal.
     *
     * @param step Size of one grid cell.
     * @param dt   Delta time.
     * @param id   Signal identifier.
     */
    void updateSignal(const PositionVector& step, units::Time dt, SignalId id);


    /**
     * @brief Update obstacle map.
     *
     * @param simulation
     */
    void updateObstacles(simulator::Simulation& simulation);


// Private Data Members
private:


    // Grid size without borders.
    SizeType m_gridSize;

    /// Signal names.
    DynamicArray<String> m_names;

    /// Diffusion rates.
    DynamicArray<DiffusionRate> m_diffusionRates;

    /// Degradation rates.
    DynamicArray<DegradationRate> m_degradationRates;

    /// Front signal grids.
    DynamicArray<GridType> m_gridsFront;

    /// Back signal grids.
    DynamicArray<GridType> m_gridsBack;

    /// Grid with obstacle data. Cannot be bool, because of std::vector<bool>.
    Grid<int> m_obstacles;

#if ENABLE_RENDER
    /// Background color.
    render::Color m_background = render::Color{0, 0, 0, 0};

    /// Signal colors.
    DynamicArray<render::Color> m_colors;

    /// Signal color saturation.
    DynamicArray<SignalConcentration> m_signalSaturation;

    /// Drawable signal grid.
#  if CONFIG_PLUGIN_diffusion_SMOOTH
    render::ObjectPtr<render::GridColorSmooth> m_drawable;
#  else
    render::ObjectPtr<render::GridColor> m_drawable;
#  endif
#endif

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
