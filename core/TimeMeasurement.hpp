
#pragma once

/* ************************************************************************ */

// C++
#include <chrono>
#include <string>
#include <ostream>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Time measurement class.
 */
class TimeMeasurement
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Measurement name.
     */
    explicit TimeMeasurement(std::string name)
        : m_name(std::move(name))
    {
        m_start = std::chrono::high_resolution_clock::now();
    }


    /**
     * @brief Destructor.
     */
    ~TimeMeasurement()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);

        *s_output << m_name << ";" << diff.count() << "\n";
    }


// Public Mutators
public:


    /**
     * @brief Change measurement output.
     *
     * @param output
     */
    static void setOutput(std::ostream* output) noexcept
    {
        s_output = output;
    }


// Private Data Members
private:

    /// Measurement name.
    std::string m_name;

    /// Measurement start
    std::chrono::high_resolution_clock::time_point m_start;

    /// Output stream.
    static std::ostream* s_output;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
