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

// C++
#include <chrono>

// CeCe
#include "cece/core/OutStream.hpp"
#include "cece/core/String.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Measurement clock.
 */
using Clock = std::chrono::high_resolution_clock;

/* ************************************************************************ */

/**
 * @brief Returns current output stream to measurement output.
 *
 * @return
 */
OutStream* getMeasureTimeOutput() noexcept;

/* ************************************************************************ */

/**
 * @brief Change output stream to measurement output.
 *
 * @param output
 */
void setMeasureTimeOutput(OutStream* output) noexcept;

/* ************************************************************************ */

/**
 * @brief Default functor for writing measurement output.
 */
struct DefaultMeasurementOutput
{

    /**
     * @brief Functor function.
     *
     * @param out  Output stream.
     * @param name Measurement name.
     * @param dt   Measured time.
     */
    void operator()(OutStream& out, const String& name, Clock::duration dt) const noexcept
    {
        using namespace std::chrono;
        out << name << ";" << duration_cast<microseconds>(dt).count() << "\n";
    }
};

/* ************************************************************************ */

/**
 * @brief Time measurement class.
 */
template<typename OutFn>
class TimeMeasurementBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Measurement name.
     * @param out  Output function.
     */
    explicit TimeMeasurementBase(String name, OutFn out)
        : m_name(std::move(name))
        , m_outFn(std::move(out))
    {
        m_start = Clock::now();
    }


    /**
     * @brief Destructor.
     */
    ~TimeMeasurementBase()
    {
        if (getMeasureTimeOutput())
        {
            auto end = Clock::now();
            m_outFn(*getMeasureTimeOutput(), m_name, end - m_start);
        }
    }


// Private Data Members
private:

    /// Measurement name.
    String m_name;

    /// Measurement start
    Clock::time_point m_start;

    /// Output function.
    OutFn m_outFn;
};

/* ************************************************************************ */

#if !ENABLE_MEASUREMENT

/**
 * @brief Dummy struct for time measurement that doesn't invoke unused variable
 * warnings.
 */
struct TimeMeasurementDummy
{
    TimeMeasurementDummy() noexcept {}
    ~TimeMeasurementDummy() {}
};

/* ************************************************************************ */

/**
 * @brief Dummy function for time measurement.
 *
 * @param args
 *
 * @return
 */
template<typename... Args>
inline TimeMeasurementDummy measure_time(Args&&... args) noexcept
{
    return {};
}
#endif

/* ************************************************************************ */

#if ENABLE_MEASUREMENT
/**
 * @brief Measure time for current statement block.
 *
 * @param name Measurement name.
 * @param fn   Output function.
 *
 * @return
 */
template<typename Fn>
inline TimeMeasurementBase<Fn> measure_time(String name, Fn fn) noexcept
{
    return TimeMeasurementBase<Fn>{std::move(name), fn};
}
#endif

/* ************************************************************************ */

#if ENABLE_MEASUREMENT
/**
 * @brief Measure time for current statement block.
 *
 * @param name Measurement name.
 *
 * @return
 */
inline TimeMeasurementBase<DefaultMeasurementOutput> measure_time(String name) noexcept
{
    return TimeMeasurementBase<DefaultMeasurementOutput>{std::move(name), DefaultMeasurementOutput{}};
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
