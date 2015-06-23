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
#include <chrono>

// Simulator
#include "core/compatibility.hpp"
#include "core/OutStream.hpp"
#include "core/String.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

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
OutStream* getMeasureTimeOutput() NOEXCEPT;

/* ************************************************************************ */

/**
 * @brief Change output stream to measurement output.
 *
 * @param output
 */
void setMeasureTimeOutput(OutStream* output) NOEXCEPT;

/* ************************************************************************ */

/**
 * @brief Default functor for writing measurement output.
 */
struct DLL_EXPORT DefaultMeasurementOutput
{

    /**
     * @brief Functor function.
     *
     * @param out  Output stream.
     * @param name Measurement name.
     * @param dt   Measured time.
     */
    void operator()(OutStream& out, const String& name, Clock::duration dt) const NOEXCEPT
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
    TimeMeasurementDummy() NOEXCEPT {}
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
inline TimeMeasurementDummy measure_time(Args&&... args) NOEXCEPT
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
inline TimeMeasurementBase<Fn> measure_time(String name, Fn fn) NOEXCEPT
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
inline TimeMeasurementBase<DefaultMeasurementOutput> measure_time(String name) NOEXCEPT
{
    return TimeMeasurementBase<DefaultMeasurementOutput>{std::move(name), DefaultMeasurementOutput{}};
}
#endif

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
