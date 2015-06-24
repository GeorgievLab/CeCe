
/* ************************************************************************ */

// Declaration
#include "Lattice.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

void Lattice::setSize(Size size)
{
    // Data requires 1 cell margin
    m_data.resize(size + 2);
    m_dataTmp.resize(size + 2);
}

/* ************************************************************************ */

void Lattice::collide(LatticeData::ValueType omega)
{
    /*
    // Lattice data is linearized and we don't need cell coordinates, so
    // foreach data as an array.
    for (auto& cell : m_data)
    {
        // Update cell values for the next step
        cell.collide(omega);
    }
    */
    const auto size = m_data.getSize();

    for (SizeType y = 1; y < size.getHeight() - 1; ++y)
    {
        for (SizeType x = 1; x < size.getWidth() - 1; ++x)
        {
            m_data[{x, y}].collide(omega);
        }
    }
}

/* ************************************************************************ */

void Lattice::propagate()
{
    // Clear result grid
    for (auto& cell : m_dataTmp)
        cell.clear();

    // TODO: linearize direction velocities
    const auto size = m_data.getSize();

    for (SizeType y = 1; y < size.getHeight() - 1; ++y)
    {
        for (SizeType x = 1; x < size.getWidth() - 1; ++x)
        {
            const Vector<LatticeData::IndexType> coord{x, y};

            for (LatticeData::IndexType i = 0; i < LatticeData::SIZE; ++i)
            {
                // Calculate new coordinates
                Vector<LatticeData::IndexType> newCoord = coord + LatticeData::DIRECTION_VELOCITIES[i];
                m_dataTmp[newCoord][i] = m_data[coord][i];
            }

            m_dataTmp[coord].setStaticObstacle(m_data[coord].isStaticObstacle());
        }
    }

    std::swap(m_data, m_dataTmp);
}

/* ************************************************************************ */

void Lattice::clearDynamicObstacles()
{
    for (auto& cell : m_data)
        cell.setDynamicObstacle(false);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
