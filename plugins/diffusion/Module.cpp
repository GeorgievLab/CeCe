
// Declaration
#include "Module.hpp"

// C++
#include <limits>
#include <functional>

// Simulator
#include "core/StaticMatrix.hpp"
#include "core/constants.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

Module::Module()
{
	m_colors.push_back(render::colors::CYAN);
	m_colors.push_back(render::colors::MAGENTA);
	m_colors.push_back(render::colors::YELLOW);
	m_colors.push_back(render::colors::BLUE);
	m_colors.push_back(render::colors::RED);
	m_colors.push_back(render::colors::GREEN);
	m_colors.push_back(render::Color{ 1, 0.894f, 0.769f });
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion", [&simulation](std::ostream& out, const std::string& name, Clock::duration dt) {
        out << name << ";" << simulation.getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
    });

    // Size of mapping matrix
    CONSTEXPR_CONST unsigned OFFSET = 1;
    CONSTEXPR_CONST unsigned MATRIX_SIZE = 2 * OFFSET + 1;

    // Precompute values
    const auto step = simulation.getWorldSize() / units::Length(1) / m_grid.getSize();

    //const float A = 1.f / (4 * constants::PI * D * dt);
    const Coefficients A = 1.f / (4 * constants::PI * getCoefficients() * dt.value());

    // Offset coefficients for matrix
    static const auto DISTANCES = StaticMatrix<int, MATRIX_SIZE>::makeDistances();

    // Sizes must match
    assert(std::distance(m_grid.begin(), m_grid.end()) == std::distance(m_gridBack.begin(), m_gridBack.end()));

    // Generate matrix with coefficients based on distance
    // TODO: precompute matrix
    const auto q = StaticMatrix<float, MATRIX_SIZE>::generate([&step](size_t i, size_t j) {
        return (step * DISTANCES[i][j]).getLengthSquared();
    });

    // Create distribution matrix
    const auto M = StaticMatrix<Coefficients, MATRIX_SIZE>::generate([&](size_t i, size_t j) {
        using std::exp;
        return A * exp(-q[i][j] / (4.f * getCoefficients() * dt.value()));
        //return A * exp(-q[i][j] / (4.f * D * dt));
        //return A * exp(-q[i][j] / dt);
    }).normalize();


    // Clear back grid
    std::fill(m_gridBack.begin(), m_gridBack.end(), Signal{});

    /*
     * Algorithm:
     * Foreach whole grid in single for loop - grid storage is 1D array.
     * This solution remove requirements for computation array offset each time
     * for reading and writing.
     * Diffusion mapping matrix contains offsets from current position to
     * the coresponding positions. Mapping matrix can be constructed as:
     * to right: +1; to left: -1; up: -width; down: +width. For 3x3 it looks:
     *   | -width - 1, -width, -width + 1 |
     *   |         -1       0,         +1 |
     *   | +width - 1, +width, +width + 1 |
     *
     * This solutions improve range checking as well. Because range is just
     * [0, width * height), only one range check is required.
     */

    const int width = m_grid.getSize().getWidth();
    const int height = m_grid.getSize().getHeight();

    const StaticMatrix<int, 3> MAPPING_MATRIX{ {
        {-width - 1, -width, -width + 1},
        {       - 1,      0,        + 1},
        {+width - 1, +width, +width + 1}
    }};

    // Current column
    int column = 0;
    int row = 0;

    // Foreach grid cells
    auto gridPtr = m_grid.cbegin();
    auto gridEndPtr = m_grid.cend();
    auto gridNewPtr = m_gridBack.begin();

    // Grid sizes are same, so we don't have to check both ranges
    for (; gridPtr != gridEndPtr; ++gridPtr, ++gridNewPtr)
    {
        // Get current signal
        const auto signal = *gridPtr;

        // No signal to send
        if (signal)
        {
            // TODO: optimize
            for (unsigned a = row < OFFSET ? OFFSET - row : 0;
                 a < ((height - row) <= OFFSET ? MATRIX_SIZE - (height - row) : MATRIX_SIZE);
                 ++a)
            {
                for (unsigned b = column < OFFSET ? OFFSET - column : 0;
                     b < ((width - column) <= OFFSET ? MATRIX_SIZE - (width - column) : MATRIX_SIZE);
                     ++b)
                {
                    auto ptr = gridNewPtr + MAPPING_MATRIX[a][b];

                    assert(ptr >= m_gridBack.begin() && ptr < m_gridBack.end());
                    *ptr += signal * M[a][b];
                }
            }
        }

        if (++column == width)
        {
            column = 0;
            ++row;
        }

    }

    {
        // Swap grids
        std::swap(m_grid, m_gridBack);
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Grid size
    config.callIfSetString("grid", [this](const std::string& value) {
        setSize(parser::parse_vector_single<unsigned int>(value));
    });

    // Diffusion coefficients
    config.callIfSetString("coefficients", [this](const std::string& value) {
        const auto coefficients = parser::parse_array<float, Signal::COUNT>(value, parser::parse_number);

        for (unsigned int i = 0; i < Signal::COUNT; ++i)
            setCoefficient(i, coefficients[i]);
    });

#if ENABLE_RENDER
    config.callIfSetString("background", [this](const std::string& value) {
        m_background = parser::parse_color(value);
    });

    config.callIfSetString("colors", [this](const std::string& value) {
        const auto colors = parser::parse_array<render::Color, Signal::COUNT>(value, parser::parse_color);

        for (unsigned int i = 0; i < Signal::COUNT; ++i)
            m_colors[i] = colors[i];
    });
#endif
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (!m_drawable)
        m_drawable.create(context, m_grid.getSize());

    updateDrawable();

    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    m_drawable->draw(context);
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::updateDrawable()
{
    assert(m_grid.getSize() == m_drawable->getSize());

    const auto size = m_grid.getSize();

    for (unsigned int y = 0u; y < size.getHeight(); ++y)
    {
        for (unsigned int x = 0u; x < size.getWidth(); ++x)
        {
            const auto& signal = m_grid[{x, y}];
            auto pixel = m_drawable->get({x, y});
            // Clear pixel
            pixel = m_background;

            // Mixup signal colors
            for (unsigned int i = 0; i < Signal::COUNT; ++i)
            {
                pixel *= (1 - signal[i]);
                pixel += m_colors[i % m_colors.size()] * signal[i];
            }

            m_drawable->set({x, y}, pixel);
        }
    }
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
