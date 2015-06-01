
#pragma once

/* ************************************************************************ */

// C++
#include <chrono>
#include <string>
#include <ostream>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

namespace core {

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
std::ostream* getMeasureTimeOutput() NOEXCEPT;

/* ************************************************************************ */

/**
 * @brief Change output stream to measurement output.
 *
 * @param output
 */
void setMeasureTimeOutput(std::ostream* output) NOEXCEPT;

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
    void operator()(std::ostream& out, const std::string& name, Clock::duration dt) const NOEXCEPT
    {
        out << name << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
    }
};

/* ************************************************************************ */

/**
 * @brief Time measurement class.
 */
template<typename OutFn>
class DLL_EXPORT TimeMeasurementBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Measurement name.
     * @param out  Output function.
     */
    explicit TimeMeasurementBase(std::string name, OutFn out)
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
    std::string m_name;

    /// Measurement start
    Clock::time_point m_start;

    /// Output function.
    OutFn m_outFn;
};

/* ************************************************************************ */

#if !ENABLE_MEASUREMENT
/**
 * @brief Dummy function for time measurement.
 *
 * @param args
 *
 * @return
 */
template<typename... Args>
inline int measure_time(Args&&... args) NOEXCEPT
{
    return 0;
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
inline TimeMeasurementBase<Fn> measure_time(std::string name, Fn fn) NOEXCEPT
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
inline TimeMeasurementBase<DefaultMeasurementOutput> measure_time(std::string name) NOEXCEPT
{
    return TimeMeasurementBase<DefaultMeasurementOutput>{std::move(name), DefaultMeasurementOutput{}};
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
