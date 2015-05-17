
/* ************************************************************************ */

// Declaration
#include "wrapper_simulator.hpp"

// Boost
#include <boost/python/class.hpp>

// Simulation
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

Vector<units::Length> get_simulation_size(simulator::Simulation* sim)
{
    return sim->getWorldSize();
}

/* ************************************************************************ */

void python_wrapper_simulator()
{
    namespace py = boost::python;

    // Configuration
    py::class_<simulator::ConfigurationBase, boost::noncopyable>("Configuration", py::no_init)
        .def("getString", &simulator::ConfigurationBase::getString)
        .def("getFloat", &simulator::ConfigurationBase::getFloat)
        .def("getInteger", &simulator::ConfigurationBase::getInteger)
    ;

    // Simulation
    py::class_<simulator::Simulation, boost::noncopyable>("Simulation", py::no_init)
        .add_property("worldSize", &get_simulation_size,
            static_cast<void(simulator::Simulation::*)(Vector<units::Length>)>(&simulator::Simulation::setWorldSize))
    ;
}

/* ************************************************************************ */
