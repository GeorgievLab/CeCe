/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// CeCe
#include "cece/config.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Grid.hpp"
#include "cece/core/IteratorRange.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/module/Module.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Color.hpp"
#  include "cece/render/Context.hpp"
#  include "cece/render/Object.hpp"
#  include "cece/render/Image.hpp"
#  include "cece/render/State.hpp"
#  if CONFIG_PLUGIN_diffusion_SMOOTH
#    include "cece/render/GridColorSmooth.hpp"
#  else
#    include "cece/render/GridColor.hpp"
#  endif
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
    static constexpr Coordinate::ValueType OFFSET = 1;

    /// Size of mapping matrix
    static constexpr Coordinate::ValueType MATRIX_SIZE = 2 * OFFSET + 1;

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
        return range<SignalId>(static_cast<SignalId>(0u), static_cast<SignalId>(getSignalCount()));
    }


    /**
     * @brief Returns signal identifier.
     *
     * @param name Signal name.
     *
     * @return Signal identifier or INVALID_SIGNAL_ID
     */
    SignalId getSignalId(StringView name) const noexcept;


    /**
     * @brief Returns signal identifier and throw if not exists.
     *
     * @param name Signal name.
     *
     * @return Signal identifier.
     */
    SignalId requireSignalId(StringView name) const
    {
        const auto id = getSignalId(name);

        if (id == INVALID_SIGNAL_ID)
            throw InvalidArgumentException("Unknown signal name: " + String(name));

        return id;
    }


    /**
     * @brief Returns signal name.
     *
     * @param id Signal identifier.
     *
     * @return Signal name or empty string.
     */
    StringView getSignalName(SignalId id) const noexcept
    {
        return (id < m_signals.size()) ? m_signals[id].name : StringView{};
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
    SignalConcentration& getSignalFront(StringView name, const Coordinate& coord)
    {
        return getSignalFront(requireSignalId(name), coord);
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignalFront(StringView name, const Coordinate& coord) const
    {
        return getSignalFront(requireSignalId(name), coord);
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
    SignalConcentration& getSignalBack(StringView name, const Coordinate& coord)
    {
        return getSignalBack(requireSignalId(name), coord);
    }


    /**
     * @brief Returns immutable signal value at given coordinates.
     *
     * @param name  Signal name.
     * @param coord Coordinates.
     *
     * @return
     */
    const SignalConcentration& getSignalBack(StringView name, const Coordinate& coord) const
    {
        return getSignalBack(requireSignalId(name), coord);
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
    SignalConcentration& getSignal(StringView name, const Coordinate& coord)
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
    const SignalConcentration& getSignal(StringView name, const Coordinate& coord) const
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
        return m_signals[id].diffusionRate;
    }


    /**
     * @brief Returns signal diffussion rate.
     *
     * @param name Signal name.
     *
     * @return
     */
    DiffusionRate getDiffusionRate(StringView name) const
    {
        return getDiffusionRate(requireSignalId(name));
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
        return m_signals[id].degradationRate;
    }


    /**
     * @brief Returns signal degradation rate.
     *
     * @param name Signal name.
     *
     * @return
     */
    DegradationRate getDegradationRate(StringView name) const
    {
        return getDegradationRate(requireSignalId(name));
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
        return m_signals[id].color;
    }


    /**
     * @brief Get signal color.
     *
     * @param name Signal name.
     *
     * @return Signal color.
     */
    const render::Color& getSignalColor(StringView name) const
    {
        return getSignalColor(requireSignalId(name));
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
        return m_signals[id].saturation;
    }


    /**
     * @brief Get signal saturation (only for rendering).
     *
     * @param name Signal name.
     *
     * @return
     */
    SignalConcentration getSignalSaturation(StringView name) const
    {
        return getSignalSaturation(requireSignalId(name));
    }


    /**
     * @brief Get signal visualization layer (only for rendering).
     *
     * @param id Signal identifier.
     *
     * @return
     */
    const String& getSignalVisualizationLayer(SignalId id) const noexcept
    {
        return m_signals[id].visualizationLayer;
    }


    /**
     * @brief Get signal visualization layer (only for rendering).
     *
     * @param name Signal name.
     *
     * @return
     */
    const String& getSignalVisualizationLayer(StringView name) const
    {
        return getSignalVisualizationLayer(requireSignalId(name));
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


    /**
     * @brief Check if there is an obstacle at given coordinates.
     *
     * @param coord
     *
     * @return
     */
    bool isObstacle(const Coordinate& coord) const noexcept
    {
        return m_obstacles[coord + OFFSET] != 0;
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
    void setSignalFront(StringView name, const Coordinate& coord, SignalConcentration value)
    {
        const auto id = requireSignalId(name);

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
    void setSignalBack(StringView name, const Coordinate& coord, SignalConcentration value)
    {
        const auto id = requireSignalId(name);

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
    void setSignal(StringView name, const Coordinate& coord, SignalConcentration value)
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
        m_signals[id].diffusionRate = rate;
    }


    /**
     * @brief Set signal diffussion rate.
     *
     * @param name Signal name.
     * @param rate Diffusion rate.
     */
    void setDiffusionRate(StringView name, DiffusionRate rate)
    {
        setDiffusionRate(requireSignalId(name), rate);
    }


    /**
     * @brief Set signal degradation rate.
     *
     * @param id   Signal identifier.
     * @param rate Degradation rate.
     */
    void setDegradationRate(SignalId id, DegradationRate rate) noexcept
    {
        m_signals[id].degradationRate = rate;
    }


    /**
     * @brief Set signal degradation rate.
     *
     * @param name Signal name.
     * @param rate Degradation rate.
     */
    void setDegradationRate(StringView name, DegradationRate rate)
    {
        setDegradationRate(requireSignalId(name), rate);
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
        m_signals[id].color = color;
    }


    /**
     * @brief Set signal color.
     *
     * @param name  Signal name.
     * @param color New signal color.
     */
    void setSignalColor(StringView name, render::Color color)
    {
        setSignalColor(requireSignalId(name), color);
    }


    /**
     * @brief Change signal saturation.
     *
     * @param id         Signal identifier.
     * @param saturation Signal value when color is max.
     */
    void setSignalSaturation(SignalId id, SignalConcentration saturation) noexcept
    {
        m_signals[id].saturation = saturation;
    }


    /**
     * @brief Change signal saturation.
     *
     * @param name       Signal name.
     * @param saturation Signal value when color is max.
     */
    void setSignalSaturation(StringView name, SignalConcentration saturation)
    {
        setSignalSaturation(requireSignalId(name), saturation);
    }


    /**
     * @brief Set signal visualization layer.
     *
     * @param id    Signal identifier.
     * @param layer Layer name.
     */
    void setSignalVisualizationLayer(SignalId id, String layer) noexcept
    {
        m_signals[id].visualizationLayer = std::move(layer);
    }


    /**
     * @brief Change signal visualization layer.
     *
     * @param name  Signal name.
     * @param layer Layer name.
     */
    void setSignalVisualizationLayer(StringView name, String layer)
    {
        setSignalVisualizationLayer(requireSignalId(name), std::move(layer));
    }

#endif


    /**
     * @brief Set obstacle flag.
     *
     * @param coord
     * @param flag
     */
    void setObstacle(const Coordinate& coord, bool flag) noexcept
    {
        m_obstacles[coord + OFFSET] = flag;
    }


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
     * @brief Initialize module.
     */
    void init() override;


    /**
     * @brief Update module state.
     */
    void update() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param visualization Visualization context.
     * @param context       Rendering context.
     */
    void draw(const simulator::Visualization& visualization, render::Context& context) override;


    /**
     * @brief Store current state for drawing.
     * State should be stored in back state because the front state is
     * used for rendering.
     * Drawing state should contain data that can be modified during update()
     * call and are used for rendering.
     * @param visualization Visualization context.
     */
    void drawStoreState(const simulator::Visualization& visualization) override;


    /**
     * @brief Swap render state.
     * Calling this function should be guarded by mutex for all modules
     * to ensure all modules are in same render state.
     * Function should be fast because otherwise it will block rendering.
     */
    void drawSwapState() override;

#endif


    /**
     * @brief Swap signal grids.
     *
     * @param id Signal identifier.
     */
    void swap(SignalId id) noexcept;


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
    void clearFront(SignalId id) noexcept;


    /**
     * @brief Clear back grid.
     *
     * @param id Signal identifier.
     */
    void clearBack(SignalId id) noexcept;


// Protected Operations
protected:


    /**
     * @brief Update signal.
     *
     * @param id Signal identifier.
     */
    virtual void updateSignal(SignalId id);


    /**
     * @brief Update obstacle map.
     */
    virtual void updateObstacles();

#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Update drawable.
     *
     * @param img           Image to update.
     * @param visualization Visualization context.
     */
    void updateImage(render::Image& img, const simulator::Visualization& visualization) const;

#endif

// Private Structures
private:


    /**
     * @brief Signal description structure.
     */
    struct Signal
    {
        /// Signal name.
        String name;

        /// Signal diffusion rate.
        DiffusionRate diffusionRate;

        /// Signal degradation rate.
        DegradationRate degradationRate;

#ifdef CECE_ENABLE_RENDER

        /// Signal visualization color.
        render::Color color;

        /// Signal visualization saturation.
        SignalConcentration saturation;

        /// Signal visualization layer.
        String visualizationLayer;
#endif
    };

// Private Structures
private:

#ifdef CECE_ENABLE_RENDER
    struct RenderState
    {
        /// Image scale.
        units::ScaleVector scale;

        /// Image to render.
        render::Image image;
    };
#endif

// Private Data Members
private:


    // Grid size without borders.
    SizeType m_gridSize;

    /// Supported signals.
    DynamicArray<Signal> m_signals;

    /// Front signal grids.
    DynamicArray<GridType> m_gridsFront;

    /// Back signal grids.
    DynamicArray<GridType> m_gridsBack;

    /// Grid with obstacle data. Cannot be bool, because of std::vector<bool>.
    Grid<int> m_obstacles;

#ifdef CECE_ENABLE_RENDER
    /// Background color.
    render::Color m_background = render::Color{0, 0, 0, 0};

    /// Render state.
    render::State<RenderState> m_drawableState;

    /// Drawable signal grid.
#  if CONFIG_PLUGIN_diffusion_SMOOTH
    render::ObjectPtr<render::GridColorSmooth> m_drawable;
#  else
    render::ObjectPtr<render::GridColor> m_drawable;
#  endif
#endif

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
