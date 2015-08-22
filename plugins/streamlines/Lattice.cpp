/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

// Declaration
#include "Lattice.hpp"

// Simulator
#include "core/VectorRange.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

void Lattice::setSize(Size size)
{
    // Data requires 1 cell margin
    m_dataFront.resize(size + 2);
    m_dataBack.resize(size + 2);
}

/* ************************************************************************ */

void Lattice::collide(LatticeData::ValueType omega)
{
    for (auto&& c : range(getSize()))
        get(c).collide(omega);
}

/* ************************************************************************ */

void Lattice::propagate()
{
    // Clear result grid
    for (auto& cell : m_dataBack)
        cell.clear();

    for (auto&& c : range(getSize()))
    {
        for (LatticeData::IndexType i = 0; i < LatticeData::SIZE; ++i)
        {
            // Calculate new coordinates
            Vector<LatticeData::IndexType> newCoord = c + LatticeData::DIRECTION_VELOCITIES[i];
            getBack(newCoord)[i] = getFront(c)[i];
        }

        getBack(c).setStaticObstacle(getFront(c).isStaticObstacle());
    }

    std::swap(m_dataFront, m_dataBack);
}

/* ************************************************************************ */

void Lattice::clearDynamicObstacles()
{
    for (auto& cell : m_dataFront)
        cell.setDynamicObstacle(false);
}

/* ************************************************************************ */

void Lattice::clearStaticObstacles()
{
    for (auto& cell : m_dataFront)
        cell.setStaticObstacle(false);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
