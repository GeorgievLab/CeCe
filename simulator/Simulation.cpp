
/* ************************************************************************ */

// Declaration
#include "simulator/Simulation.hpp"

// C++
#include <algorithm>
#include <tuple>

// Simulator
#include "core/Log.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Library.hpp"
#include "simulator/Simulator.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

std::tuple<std::string, std::string> splitModulePath(const std::string& path) noexcept
{
    // Find dot separator
    auto pos = path.find('.');

    if (pos == std::string::npos)
        return std::make_tuple(path, std::string{});
    else
        return std::make_tuple(path.substr(0, pos), path.substr(pos + 1));
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Simulation::Simulation(Simulator& simulator) noexcept
    : m_simulator(simulator)
#ifdef ENABLE_PHYSICS
    , m_world{b2Vec2{0.0f, 0.0f}}
#endif
{
    // Nothing to do
}

/* ************************************************************************ */

Simulation::~Simulation()
{
    // Delete objects (before physics world)
    m_objects.clear();
}

/* ************************************************************************ */

Module* Simulation::useModule(const std::string& path)
{
    // Module exists, return the existing one
    if (hasModule(path))
        return getModule(path);

    // Split path into parts
    std::string library, name;
    std::tie(library, name) = splitModulePath(path);

    // Load library
    Library* lib = m_simulator.loadLibrary(library);
    assert(lib);

    // Unable to load library
    if (!lib->isLoaded())
    {
        Log::warning("Unable to load library: ", library, "(", lib->getError(), ")");
        return nullptr;
    }

    // Create module with given name
    auto module = lib->createModule(this, name);

    // Register module
    if (module)
    {
        Log::info("Using module: ", path);
        return addModule(path, std::move(module));
    }

    Log::warning("Unable to create module: ", path, " (unsupported by library?)");

    return nullptr;
}

/* ************************************************************************ */

void Simulation::reset()
{
    m_stepNumber = 0;
}

/* ************************************************************************ */

void Simulation::update(units::Duration dt) noexcept
{
    // Increase step number
    m_stepNumber++;

    // Update modules
    {
        auto _ = measure_time("sim.modules", [this](std::ostream& out, const std::string& name, Clock::duration dt) {
            out << name << ";" << getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
        });

        for (auto& module : getModules())
            module.second->update(dt, *this);
    }

    {
        auto _ = measure_time("sim.objects", [this](std::ostream& out, const std::string& name, Clock::duration dt) {
            out << name << ";" << getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
        });

        // Update simulations objects
        for (std::size_t i = 0; i < getObjects().size(); ++i)
        {
            auto& obj = getObjects()[i];
            assert(obj);
            obj->update(dt);
        }
    }

    // Remove objects that are outside world.
    {
        auto _ = measure_time("sim.delete", [this](std::ostream& out, const std::string& name, Clock::duration dt) {
            out << name << ";" << getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
        });

        const auto hh = getWorldSize() * 0.5f;

        // Kill objects that are outside world
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [&hh](const ObjectContainer::value_type& obj) {
            // Ignore static objects
            if (obj->getType() == Object::Type::Static)
                return false;

            // Get object position
            const auto& pos = obj->getPosition();

            // TODO: optimize
            return not (
                ((pos.getX() >= -hh.getX()) && (pos.getX() <= hh.getX())) &&
                ((pos.getY() >= -hh.getY()) && (pos.getY() <= hh.getY()))
            );
        }), m_objects.end());

        // Remove objects from drawInit
        m_drawInitList.erase(std::remove_if(m_drawInitList.begin(), m_drawInitList.end(), [this](Object* ptr) {
            return std::find_if(m_objects.begin(), m_objects.end(), [ptr](const ObjectContainer::value_type& obj) {
                return obj.get() == ptr;
            }) == m_objects.end();
        }), m_drawInitList.end());
    }

#ifdef ENABLE_PHYSICS
    {
        auto _ = measure_time("sim.physics", [this](std::ostream& out, const std::string& name, Clock::duration dt) {
            out << name << ";" << getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
        });

        m_world.Step(dt, 1, 1);
    }
#endif
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulation::drawInit(render::Context& context)
{
    // Init modules for rendering
    for (auto& module : getModules())
        module.second->drawInit(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulation::draw(render::Context& context)
{
    context.setStencilBuffer(getWorldSize().getWidth(), getWorldSize().getHeight());

#if ENABLE_PHYSICS_DEBUG
    m_world.DrawDebugData();
#endif

    // Render modules
    for (auto& module : getModules())
        module.second->draw(context, *this);

    // Prepare objects for rendering
    {
        for (auto ptr : m_drawInitList)
        {
            assert(ptr);
            ptr->drawInit(context);
        }

        m_drawInitList.clear();
    }

    // Draw lines around world
    {
        const auto hw = getWorldSize().getWidth() * 0.5f;
        const auto hh = getWorldSize().getHeight() * 0.5f;
        const auto sw = getWorldSize().getWidth() / 16.f;
        const auto sh = getWorldSize().getHeight() / 16.f;

        const render::Color color = {0.3, 0.3, 0.3, 0.2};

        context.drawLine({-hw, -hh}, {0, sh}, color);
        context.drawLine({-hw, -hh}, {sw, 0}, color);

        context.drawLine({hw, -hh}, {0, sh}, color);
        context.drawLine({hw, -hh}, {-sw, 0}, color);

        context.drawLine({-hw, hh}, {0, -sh}, color);
        context.drawLine({-hw, hh}, {sw, 0}, color);

        context.drawLine({hw, hh}, {0, -sh}, color);
        context.drawLine({hw, hh}, {-sw, 0}, color);
    }

    // Draw objects
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->draw(context);
    }
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
