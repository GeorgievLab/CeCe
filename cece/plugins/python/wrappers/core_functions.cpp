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

// Must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/UnitsCtors.hpp"

// Plugin
#include "cece/plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
PyObject* m(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::m(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
PyObject* mm(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mm(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrometer value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
PyObject* um(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::um(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Kilograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
PyObject* kg(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::kg(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Grams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
PyObject* g(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::g(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Milligrams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
PyObject* mg(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mg(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
PyObject* ug(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::ug(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Nanograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
PyObject* ng(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::ng(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
PyObject* pg(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::pg(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Seconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
PyObject* s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Milliseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
PyObject* ms(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::ms(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Microseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
PyObject* us(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::us(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Minutes value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
PyObject* min(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::min(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Hours value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
PyObject* h(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::h(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Meters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
PyObject* m2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::m2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimeters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
PyObject* mm2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mm2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrometers^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
PyObject* um2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::um2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Meters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
PyObject* m3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::m3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimeters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
PyObject* mm3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mm3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
PyObject* um3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::um3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Meters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
PyObject* m_s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::m_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
PyObject* mm_s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mm_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
PyObject* um_s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::um_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Meters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
PyObject* m_s2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::m_s2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
PyObject* mm_s2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mm_s2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
PyObject* um_s2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::um_s2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Kilogram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
PyObject* kgm_s2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::kgm_s2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Gram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
PyObject* gm_s2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::gm_s2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Milligram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
PyObject* mgm_s2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mgm_s2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Newton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
PyObject* N(PyObject* self, PyObject* args) noexcept
{
    return kgm_s2(self, args);
}

/* ************************************************************************ */

/**
 * @brief Millinewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
PyObject* mN(PyObject* self, PyObject* args) noexcept
{
    return gm_s2(self, args);
}

/* ************************************************************************ */

/**
 * @brief Micronewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
PyObject* uN(PyObject* self, PyObject* args) noexcept
{
    return mgm_s2(self, args);
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* kg_ms(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::kg_m_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* g_ms(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::g_m_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* Ns_m2(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::Ns_m2(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* Pas(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::Pas(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* mPas(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mPas(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Meter square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* m2_s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::m2_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimeter square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* mm2_s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mm2_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micrometer square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
PyObject* um2_s(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::um2_s(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Mole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
PyObject* mol(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mol(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Millimole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
PyObject* mmol(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mmol(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Micromole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
PyObject* umol(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::umol(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Nanomole value.
 *
 * @param value
 *
 * @return Amount of substance.
 */
PyObject* nmol(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::nmol(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in mole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* mol_m3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mol_m3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in millimole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* mmol_m3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mmol_m3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in micromole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* umol_m3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::umol_m3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in nanomole per meter cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* nmol_m3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::nmol_m3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in mole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* mol_um3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mol_um3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in millimole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* mmol_um3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::mmol_um3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in micromole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* umol_um3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::umol_um3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Returns concentration in nanomole per micrometer cubic.
 *
 * @param value
 *
 * @return Molar concentration.
 */
PyObject* nmol_um3(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::nmol_um3(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Radian constructor.
 *
 * @param value Value in radians.
 *
 * @return Angle.
 */
PyObject* rad(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::rad(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Degree constructor.
 *
 * @param value Value in degrees.
 *
 * @return Angle.
 */
PyObject* deg(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::deg(value)).release();
}

/* ************************************************************************ */

/**
 * @brief Probability constructor.
 *
 * @param value Value in precent.
 *
 * @return
 */
PyObject* precent(PyObject* self, PyObject* args) noexcept
{
    float value;

    if (!PyArg_ParseTuple(args, "f", &value))
        return nullptr;

    return makeObject(units::precent(value)).release();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

PyMethodDef core_functions[] = {
    {"m",       m, METH_VARARGS, nullptr},
    {"mm",      mm, METH_VARARGS, nullptr},
    {"um",      um, METH_VARARGS, nullptr},
    {"kg",      kg, METH_VARARGS, nullptr},
    {"g",       g, METH_VARARGS, nullptr},
    {"mg",      mg, METH_VARARGS, nullptr},
    {"ug",      ug, METH_VARARGS, nullptr},
    {"ng",      ng, METH_VARARGS, nullptr},
    {"pg",      pg, METH_VARARGS, nullptr},
    {"s",       s, METH_VARARGS, nullptr},
    {"ms",      ms, METH_VARARGS, nullptr},
    {"us",      us, METH_VARARGS, nullptr},
    {"min",     min, METH_VARARGS, nullptr},
    {"h",       h, METH_VARARGS, nullptr},
    {"m2",      m2, METH_VARARGS, nullptr},
    {"mm2",     mm2, METH_VARARGS, nullptr},
    {"um2",     um2, METH_VARARGS, nullptr},
    {"m3",      m3, METH_VARARGS, nullptr},
    {"mm3",     mm3, METH_VARARGS, nullptr},
    {"um3",     um3, METH_VARARGS, nullptr},
    {"m_s",     m_s, METH_VARARGS, nullptr},
    {"mm_s",    mm_s, METH_VARARGS, nullptr},
    {"um_s",    um_s, METH_VARARGS, nullptr},
    {"m_s2",    m_s2, METH_VARARGS, nullptr},
    {"mm_s2",   mm_s2, METH_VARARGS, nullptr},
    {"um_s2",   um_s2, METH_VARARGS, nullptr},
    {"kgm_s2",  kgm_s2, METH_VARARGS, nullptr},
    {"gm_s2",   gm_s2, METH_VARARGS, nullptr},
    {"mgm_s2",  mgm_s2, METH_VARARGS, nullptr},
    {"N",       N, METH_VARARGS, nullptr},
    {"mN",      mN, METH_VARARGS, nullptr},
    {"uN",      uN, METH_VARARGS, nullptr},
    {"kg_ms",   kg_ms, METH_VARARGS, nullptr},
    {"g_ms",    g_ms, METH_VARARGS, nullptr},
    {"Ns_m2",   Ns_m2, METH_VARARGS, nullptr},
    {"Pas",     Pas, METH_VARARGS, nullptr},
    {"mPas",    mPas, METH_VARARGS, nullptr},
    {"m2_s",    m2_s, METH_VARARGS, nullptr},
    {"mm2_s",   mm2_s, METH_VARARGS, nullptr},
    {"um2_s",   um2_s, METH_VARARGS, nullptr},
    {"mol",     mol, METH_VARARGS, nullptr},
    {"mmol",    mmol, METH_VARARGS, nullptr},
    {"umol",    umol, METH_VARARGS, nullptr},
    {"nmol",    nmol, METH_VARARGS, nullptr},
    {"mol_m3",  mol_m3, METH_VARARGS, nullptr},
    {"mmol_m3", mmol_m3, METH_VARARGS, nullptr},
    {"umol_m3", umol_m3, METH_VARARGS, nullptr},
    {"nmol_m3", nmol_m3, METH_VARARGS, nullptr},
    {"mol_um3", mol_um3, METH_VARARGS, nullptr},
    {"mmol_um3", mmol_um3, METH_VARARGS, nullptr},
    {"umol_um3", umol_um3, METH_VARARGS, nullptr},
    {"nmol_um3", nmol_um3, METH_VARARGS, nullptr},
    {"rad",     rad, METH_VARARGS, nullptr},
    {"deg",     deg, METH_VARARGS, nullptr},
    {"precent", precent, METH_VARARGS, nullptr},
    {nullptr}        /* Sentinel */
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
