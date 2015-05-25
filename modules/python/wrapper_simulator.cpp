
/* ************************************************************************ */

// Declaration
#include "wrapper_simulator.hpp"

// Boost
#include <boost/python/class.hpp>

// Simulation
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

core::Vector<core::units::Length> get_simulation_size(simulator::Simulation* sim)
{
    return sim->getWorldSize();
}

/* ************************************************************************ */

static void python_wrapper_simulator_Configuration()
{
    namespace py = boost::python;

    // Configuration
    py::class_<simulator::ConfigurationBase, boost::noncopyable>("Configuration", py::no_init)
        .def("getString", &simulator::ConfigurationBase::getString)
        .def("getFloat", &simulator::ConfigurationBase::getFloat)
        .def("getInteger", &simulator::ConfigurationBase::getInteger)
    ;
}

/* ************************************************************************ */

static void python_wrapper_simulator_Simulation()
{
    namespace py = boost::python;

    // Simulation
    py::class_<simulator::Simulation, boost::noncopyable>("Simulation", py::no_init)
        .add_property("worldSize", &get_simulation_size,
            static_cast<void(simulator::Simulation::*)(core::Vector<core::units::Length>)>(&simulator::Simulation::setWorldSize))
    ;
}

/* ************************************************************************ */

void python_wrapper_simulator()
{
    python_wrapper_simulator_Configuration();
    python_wrapper_simulator_Simulation();
}

/* ************************************************************************ */
