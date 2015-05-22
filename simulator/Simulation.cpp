
/* ************************************************************************ */

// Declaration
#include "simulator/Simulation.hpp"

// C++
#include <algorithm>
#include <tuple>
#include <chrono>

// Simulator
#include "core/Log.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Library.hpp"
#include "simulator/Simulator.hpp"
#include "render/Color.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

std::tuple<std::string, std::string> splitModulePath(const std::string& path) noexcept
{
    auto pos = path.find(':');

    // Using wrapper
    if (pos != std::string::npos)
    {
        // Create wrapper version
        return std::make_tuple(path.substr(0, pos), path.substr(pos + 1));
    }
    else
    {
        // Find dot separator
        auto pos = path.find('.');

        if (pos == std::string::npos)
            return std::make_tuple(path, std::string{});
        else
            return std::make_tuple(path.substr(0, pos), path.substr(pos + 1));
    }
}

/* ************************************************************************ */

/**
 * @brief Remove objects from second list that are not in the first list.
 *
 * @param
 */
template<typename Container1, typename Container2>
void erase_mismatch(const Container1& con1, Container2& con2) noexcept
{
    using std::begin;
    using std::end;

    // Remove objects from drawInit
    con2.erase(std::remove_if(begin(con2), end(con2), [&con1](typename Container2::value_type ptr) {
        return std::find_if(begin(con1), end(con1), [ptr](const typename Container1::value_type& obj) {
            return obj.get() == ptr;
        }) == end(con1);
    }), end(con2));
}

/* ************************************************************************ */

/**
 * @brief Initialize all objects in container and after that remove all.
 *
 * @param con
 */
template<typename Container, typename Context>
void draw_init(Container& con, Context& context)
{
    for (auto ptr : con)
    {
        assert(ptr);
        ptr->drawInit(context);
    }

    con.clear();
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Simulation::Simulation() noexcept
#if ENABLE_PHYSICS
    : m_world{b2Vec2{0.0f, 0.0f}}
#endif
{
    // Nothing to do
}

/* ************************************************************************ */

Simulation::~Simulation()
{
    // Nothing to do
}

/* ************************************************************************ */

Module* Simulation::useModule(const std::string& path)
{
    // Module exists, return the existing one
    if (hasModule(path))
        return getModule(path);

    Log::debug("Loading library: ", path);

    // Split path into parts
    std::string library, name;
    std::tie(library, name) = splitModulePath(path);

    // Get API
    LibraryApi* api = getLibraryApi(library);

    // Load only library
    if (name.empty())
        Log::debug("Create module '", library, "'");
    else
        Log::debug("Create module '", library, ".", name, "'");

    // Create module with given name
    auto module = api->createModule(*this, name);

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

Object* Simulation::buildObject(const std::string& name, bool dynamic)
{
    // Split path into parts
    std::string library, type;
    std::tie(library, type) = splitModulePath(name);

    if (type.empty())
        throw std::invalid_argument("Missing object type");

    // Get API
    LibraryApi* api = getLibraryApi(library);

    Log::debug("Create object '", library, ".", type, "'");

    // Create object with given name
    auto object = api->createObject(*this, type, dynamic);

    // Register module
    if (object)
        return addObject(std::move(object));

    Log::warning("Unable to create object: ", name, " (unsupported by library?)");

    return nullptr;
}

/* ************************************************************************ */

Program Simulation::buildProgram(const std::string& path)
{
    // Split path into parts
    std::string library, type;
    std::tie(library, type) = splitModulePath(path);

    if (type.empty())
        throw std::invalid_argument("Missing program type");

    // Get API
    LibraryApi* api = getLibraryApi(library);

    Log::debug("Create program '", library, ".", type, "'");

    // Create object with given name
    return api->createProgram(*this, type);
}

/* ************************************************************************ */

void Simulation::reset()
{
    m_stepNumber = 0;
}

/* ************************************************************************ */

bool Simulation::update(units::Duration dt) noexcept
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
        erase_mismatch(m_objects, m_drawInitObjectList);
    }

#ifdef ENABLE_PHYSICS
    {
        auto _ = measure_time("sim.physics", [this](std::ostream& out, const std::string& name, Clock::duration dt) {
            out << name << ";" << getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
        });

        m_world.Step(dt, 5, 5);
    }
#endif

    return (hasUnlimitedIterations() || getStepNumber() <= getIterations());
}

/* ************************************************************************ */

bool Simulation::update()
{
    if (isTimeStepRealTime())
    {
        using clock_type = std::chrono::high_resolution_clock;
        using duration_type = std::chrono::duration<float, std::chrono::seconds::period>;

        // Last update duration
        static clock_type::duration diff{1};

        // Get start time
        auto start = clock_type::now();

        static_assert(std::chrono::treat_as_floating_point<duration_type::rep>::value, "ehm...");
        bool res = update(std::chrono::duration_cast<duration_type>(diff).count());

        // Calculate time that takes to update simulation (then use it in next step)
        diff = clock_type::now() - start;

        return res;
    }
    else
    {
        return update(getTimeStep());
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulation::draw(render::Context& context)
{
    context.setStencilBuffer(getWorldSize().getWidth(), getWorldSize().getHeight());

    // Prepare modules for rendering
    draw_init(m_drawInitModuleList, context);

    // Prepare objects for rendering
    draw_init(m_drawInitObjectList, context);

    // Render modules
    for (auto& module : getModules())
        module.second->draw(context, *this);

/*
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
*/
    // Draw objects
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->draw(context);
    }

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    if (isDrawPhysics())
        m_world.DrawDebugData();
#endif

}
#endif

/* ************************************************************************ */

Library* Simulation::loadLibrary(const std::string& name)
{
    // Try to find library in cache
    auto it = m_libraries.find(name);

    // Not found
    if (it == m_libraries.end())
    {
        // Insert into cache
        auto ptr = m_libraries.emplace(std::make_pair(
            name,
            std::unique_ptr<Library>{new Library(name)}
        ));
        it = std::get<0>(ptr);
    }

    // Return pointer
    return std::get<1>(*it).get();
}

/* ************************************************************************ */

LibraryApi* Simulation::getLibraryApi(const std::string& name)
{
    // Get if library is already loaded
    const bool init = !hasLibrary(name);

    // Load library
    Library* lib = loadLibrary(name);
    assert(lib);

    // Unable to load library
    if (!lib->isLoaded())
    {
        Log::warning("Unable to load library: ", name, "(", lib->getError(), ")");
        return nullptr;
    }

    // Get API
    LibraryApi* api = lib->getApi();
    assert(api);

    // Initialize simulation
    if (init)
    {
        Log::debug("Initialize simulation '", name, "'");
        api->initSimulation(*this);
    }

    return api;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
