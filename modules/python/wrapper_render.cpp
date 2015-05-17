
/* ************************************************************************ */

// Declaration
#include "wrapper_render.hpp"

// Boost
#include <boost/python/class.hpp>

#if ENABLE_RENDER
// Render
#include "render/Color.hpp"
#include "render/Context.hpp"
#include "render/GridColor.hpp"
#endif

/* ************************************************************************ */

void python_wrapper_render()
{
#if ENABLE_RENDER
    namespace py = boost::python;

    py::class_<render::Color>("Color")
        .def(py::init<render::Color::ComponentType>())
        .def(py::init<render::Color::ComponentType, render::Color::ComponentType, render::Color::ComponentType, render::Color::ComponentType>())
        .add_property("red", &render::Color::getRed, &render::Color::setRed)
        .add_property("green", &render::Color::getGreen, &render::Color::setGreen)
        .add_property("blue", &render::Color::getBlue, &render::Color::setBlue)
        .add_property("alpha", &render::Color::getAlpha, &render::Color::setAlpha)
    ;

    void (render::Context::*matrixScale1)(float) = &render::Context::matrixScale;
    void (render::Context::*matrixScale2)(const Vector<float>&) = &render::Context::matrixScale;

    // render::Context
    py::class_<render::Context, boost::noncopyable>("Context", py::no_init)
        .def("matrixPush", &render::Context::matrixPush)
        .def("matrixPop", &render::Context::matrixPop)
        .def("matrixIdentity", &render::Context::matrixIdentity)
        .def("matrixScale", matrixScale1)
        .def("matrixScale", matrixScale2)
    ;

    void (render::GridColor::*init_1)(render::Context&) = &render::GridColor::init;
    void (render::GridColor::*init_2)(render::Context&, Vector<render::GridColor::PositionType>) = &render::GridColor::init;

    py::class_<render::GridColor, boost::noncopyable>("GridColor")
        .def("init", init_1)
        .def("init", init_2)
        .def("draw", &render::GridColor::draw)
        .def("clear", &render::GridColor::clear)
        .def("set", &render::GridColor::set)
        //.def("get", &render::GridColor::get)
    ;

#endif
}

/* ************************************************************************ */
