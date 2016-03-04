/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// C++
#include <algorithm>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Grid.hpp"
#include "cece/core/Range.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/module/Module.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Color.hpp"
#  include "cece/render/Context.hpp"
#  include "cece/render/Object.hpp"
#  if CONFIG_PLUGIN_diffusion_SMOOTH
#    include "cece/render/GridColorSmooth.hpp"
#  else
#    include "cece/render/GridColor.hpp"
#  endif
#endif

#ifdef CECE_THREAD_SAFE
#  include "cece/core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion.
 */
class Module : public module::Module
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
    static constexpr typename Coordinate::ValueType OFFSET = 1;

    /// Invalid signal ID.
    static constexpr SignalId INVALID_SIGNAL_ID = -1;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    explicit Module(simulator::Simulation& simulation) noexcept
        : module::Module(simulation)
    {
#ifdef CECE_ENABLE_RENDER
        setZOrder(100);
#endif
    }


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
    std::size_t getSignalCount() const noexcept
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
     * @brief Returns signal identifier and throw if not exists.
     *
     * @param name Signal name.
     *
     * @return Signal identifier.
     */
    SignalId requireSignalId(const String& name) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + name);

        return id;
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


#ifdef CECE_ENABLE_RENDER

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
        return coord.inRange(Coordinate{Zero}, m_gridSize);
    }


#ifdef CECE_THREAD_SAFE

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


#ifdef CECE_ENABLE_RENDER

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
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Update module state.
     */
    void update() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;


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
     * @param id   Signal identifier.
     */
    void updateSignal(const PositionVector& step, SignalId id);


    /**
     * @brief Update obstacle map.
     */
    void updateObstacles();


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

#ifdef CECE_ENABLE_RENDER
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

#ifdef CECE_THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

    /// Outstream for diffusion data.
    UniquePtr<OutStream> m_dataOut;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
