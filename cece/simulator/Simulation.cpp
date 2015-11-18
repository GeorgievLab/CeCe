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

// Declaration
#include "cece/simulator/Simulation.hpp"

// C++
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/Tuple.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/simulator/Simulator.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/PluginManager.hpp"
#include "cece/simulator/Obstacle.hpp"

#if CONFIG_RENDER_TEXT_ENABLE
#include "cece/simulator/font.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if ENABLE_PHYSICS
/**
 * @brief AABB query callback.
 */
class QueryCallback : public b2QueryCallback
{
public:


    QueryCallback(const b2Vec2& point) noexcept
        : m_point(point)
        , m_object(nullptr)
    {
        // Nothing to do
    }


    b2Body* getObject() const noexcept
    {
        return m_object;
    }


    bool ReportFixture(b2Fixture* fixture) override
    {
        if (fixture->IsSensor())
            return true; //ignore sensors

        bool inside = fixture->TestPoint(m_point);
        if (!inside)
            return true;

         // We are done, terminate the query.
         m_object = fixture->GetBody();
         return false;
    }

// Private Data Members
private:

    b2Vec2  m_point;
    b2Body* m_object;
};
#endif

/* ************************************************************************ */

Tuple<String, String> splitModulePath(const String& path) noexcept
{
    auto pos = path.find(':');

    // Using wrapper
    if (pos != String::npos)
    {
        // Create wrapper version
        return std::make_tuple(path.substr(0, pos), path.substr(pos + 1));
    }
    else
    {
        // Find dot separator
        auto pos = path.find('.');

        if (pos == String::npos)
            return std::make_tuple(path, String{});
        else
            return std::make_tuple(path.substr(0, pos), path.substr(pos + 1));
    }
}

/* ************************************************************************ */

/**
 * @brief Write CSV line into output stream.
 *
 * @param os
 * @param container
 */
template<typename Container>
void writeCsvLine(OutStream& os, const Container& container)
{
    using std::begin;
    using std::end;

    for (auto it = begin(container); it != end(container); ++it)
    {
        if (it != begin(container))
            os << ';';

        os << *it;
    }

    os << "\n";
}

/* ************************************************************************ */

}

/* ************************************************************************ */

/**
 * @brief Read object type from stream.
 *
 * @param is   Input stream.
 * @param type
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Object::Type& type)
{
    String value;
    is >> value;

    if (value == "static")
        type = Object::Type::Static;
    else if (value == "pinned")
        type = Object::Type::Pinned;
    else
        type = Object::Type::Dynamic; // Default

    return is;
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
    // Store data tables
    storeDataTables();

    // Call finalize simulations for all plugins
    for (auto it = m_plugins.rbegin(); it != m_plugins.rend(); ++it)
    {
        assert(it->second);
        it->second->finalizeSimulation(*this);
    }
}

/* ************************************************************************ */

AccelerationVector Simulation::getGravity() const noexcept
{
#if ENABLE_PHYSICS
    const auto coeff = calcPhysicalEngineCoefficient();

    const auto grav = m_world.GetGravity();
    return {
        units::Acceleration(coeff * coeff * grav.x),
        units::Acceleration(coeff * coeff * grav.y)
    };
#else
    return AccelerationVector{Zero};
#endif
}

/* ************************************************************************ */

ViewPtr<const ObjectType> Simulation::findObjectType(const StringView& name) const noexcept
{
    auto it = std::find_if(m_objectClasses.begin(), m_objectClasses.end(), [&name] (const ObjectType& type) {
        return type.name == name;
    });

    return it != m_objectClasses.end() ? &*it : nullptr;
}

/* ************************************************************************ */

void Simulation::setGravity(const AccelerationVector& gravity) noexcept
{
#if ENABLE_PHYSICS
    const auto coeff = calcPhysicalEngineCoefficient();

    const b2Vec2 gravityPhys{
        gravity.getX().value() / (coeff * coeff),
        gravity.getY().value() / (coeff * coeff)
    };

    m_world.SetGravity(gravityPhys);
#else
    // TODO: store
#endif
}

/* ************************************************************************ */

ViewPtr<Module> Simulation::useModule(const String& path, String storePath)
{
    if (storePath.empty())
        storePath = path;

    // Module exists, return the existing one
    if (hasModule(storePath))
        return getModule(storePath);

    Log::debug("Loading library: ", path);

    // Split path into parts
    String library, name;
    std::tie(library, name) = splitModulePath(path);

    // Get API
    PluginApi* api = requirePlugin(library);

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
        if (storePath != path)
        {
            Log::info("Using module '", path, "' as '", storePath, "'");
        }
        else
        {
            Log::info("Using module '", path, "'");
        }
        return addModule(storePath, std::move(module));
    }

    Log::warning("Unable to create module '", path, "' (unsupported by library?)");

    return nullptr;
}

/* ************************************************************************ */

Object* Simulation::buildObject(const String& path, Object::Type type)
{
    // Try to find object internal object type
    auto desc = findObjectType(path);

    if (desc)
    {
        // Create parent object
        auto obj = buildObject(desc->baseName);

        // Configure
        obj->configure(desc->config, *this);

        return obj;
    }

    // Split path into parts
    String library, name;
    std::tie(library, name) = splitModulePath(path);

    if (name.empty())
        throw InvalidArgumentException("Missing object type name");

    // Get API
    PluginApi* api = requirePlugin(library);

    Log::debug("Create object '", library, ".", name, "'");

    // Create object with given name
    auto object = api->createObject(*this, name, type);

    // Register module
    if (object)
        return addObject(std::move(object));

    Log::warning("Unable to create object: ", path, " (unsupported by library?)");

    return nullptr;
}

/* ************************************************************************ */

Program Simulation::buildProgram(const String& path)
{
    // Split path into parts
    String library, type;
    std::tie(library, type) = splitModulePath(path);

    if (type.empty())
        throw InvalidArgumentException("Missing program type");

    // Get API
    PluginApi* api = requirePlugin(library);

    Log::debug("Create program '", library, ".", type, "'");

    // Create object with given name
    return api->createProgram(*this, type);
}

/* ************************************************************************ */

void Simulation::reset()
{
    m_iteration = 0;
    m_totalTime = Zero;
    m_initialized = false;
}

/* ************************************************************************ */

bool Simulation::update(units::Duration dt)
{
    // Initialize simulation
    if (!isInitialized())
        initialize();

    // Increase step number
    m_iteration++;
    m_totalTime += dt;

    // Clear all stored forces
    for (auto& obj : m_objects)
        obj->setForce(Zero);

    // Update modules
    updateModules(dt);

    // Update objects
    updateObjects(dt);

    // Detect object that leaved the scene
    detectDeserters();

    // Delete unused objects
    deleteObjects();

#ifdef ENABLE_PHYSICS
    {
        auto _ = measure_time("sim.physics", TimeMeasurementIterationOutput(this));

        m_world.Step(getPhysicsEngineTimeStep().value(), 10, 10);
    }
#endif

    return (hasUnlimitedIterations() || getIteration() <= getIterations());
}

/* ************************************************************************ */

void Simulation::initialize()
{
    assert(!isInitialized());

    for (auto& init : m_initializers)
        init(*this);

    m_initialized = true;
}

/* ************************************************************************ */

void Simulation::configure(const Configuration& config)
{
    // Resize world
    {
        auto size = config.get<SizeVector>("world-size");

        if (size.getWidth() == Zero || size.getHeight() == Zero)
            throw ConfigException("Width or height is zero!");

        setWorldSize(size);
    }

    // Time step
    setTimeStep(config.get<units::Time>("dt"));

    // Set gravity
    setGravity(config.get("gravity", getGravity()));

    // Number of iterations
    setIterations(config.get("iterations", getIterations()));

    // Background color
    setBackgroundColor(config.get("background", getBackgroundColor()));

#if CONFIG_RENDER_TEXT_ENABLE
    setFontColor(config.get("text-color", getBackgroundColor().inverted()));
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    setFontSize(config.get("text-size", getFontSize()));
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    setSimulationTimeRender(config.get("show-simulation-time", isSimulationTimeRender()));
#endif

    // Parse parameters
    for (auto&& parameterConfig : config.getConfigurations("parameter"))
    {
        setParameter(parameterConfig.get("name"), units::parse(parameterConfig.get("value")));
    }

    // Register user types
    for (auto&& typeConfig : config.getConfigurations("type"))
    {
        addObjectType({
            typeConfig.get("name"),
            typeConfig.get("base"),
            typeConfig.toMemory()
        });
    }

    // Parse init
    for (auto&& initConfig : config.getConfigurations("init"))
    {
        // Plugin is required
        auto api = requirePlugin(initConfig.get("language"));

        // Register program
        addInitializer(api->createInitializer(*this, initConfig.getContent()));
    }

    // Parse plugins
    for (auto&& pluginConfig : config.getConfigurations("plugin"))
    {
        // Returns valid pointer or throws an exception
        requirePlugin(pluginConfig.get("name"))->configure(*this, pluginConfig);
    }

    // Parse modules
    for (auto&& moduleConfig : config.getConfigurations("module"))
    {
        // Create module by given name
        auto module = useModule(
            moduleConfig.get("name"),
            moduleConfig.get("access-name", String{})
        );

        // Configure module
        if (module)
            module->loadConfig(*this, moduleConfig);
    }

    // Parse programs
    for (auto&& programConfig : config.getConfigurations("program"))
    {
        // Plugin is required
        auto api = requirePlugin(programConfig.get("language"));

        // Program name
        const auto name = programConfig.get("name");

        // Register program
        addProgram(name, api->createProgram(*this, name, programConfig.getContent()));
    }

    // Parse objects
    for (auto&& objectConfig : config.getConfigurations("object"))
    {
        // Create object
        auto object = buildObject(
            objectConfig.get("class"),
            objectConfig.get("type", Object::Type::Dynamic)
        );

        if (object)
            object->configure(objectConfig, *this);
    }

    // Parse obstacles
    for (auto&& obstacleConfig : config.getConfigurations("obstacle"))
    {
        // Create object
        auto object = createObject<simulator::Obstacle>();

        if (object)
            object->configure(obstacleConfig, *this);
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulation::draw(render::Context& context)
{
    context.setStencilBuffer(getWorldSize().getWidth().value(), getWorldSize().getHeight().value());

    // Store modules
    DynamicArray<ViewPtr<Module>> modules;
    for (auto& module : m_modules)
        modules.push_back(module.second);

    // Sort modules by rendering order
    std::sort(modules.begin(), modules.end(), [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
        return lhs->getZOrder() < rhs->getZOrder();
    });

    // Render modules
    for (auto& module : modules)
        module->draw(*this, context);

    // Draw objects
    for (auto& obj : m_objects)
    {
        Assert(obj);
        if (obj->isVisible())
            obj->draw(context);
    }

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    if (isDrawPhysics())
        m_world.DrawDebugData();
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    context.disableStencilBuffer();

    if (isSimulationTimeRender())
    {
        if (!m_font)
        {
            m_font.create(context, g_fontData);
            m_font->setSize(getFontSize());
        }

        OutStringStream oss;
        {
            auto time = getTotalTime().value();
            unsigned int seconds = time;
            unsigned int milliseconds = static_cast<unsigned int>(time * 1000) % 1000;

            const unsigned int hours = seconds / (60 * 60);
            seconds %= (60 * 60);
            const unsigned int minutes = seconds / 60;
            seconds %= 60;

            if (hours)
            {
                oss << std::setw(2) << std::setfill('0') << hours << ":";
                oss << std::setw(2) << std::setfill('0') << minutes << ":";
            }
            else if (minutes)
            {
                oss << std::setw(2) << std::setfill('0') << minutes << ":";
            }

            oss << std::setw(2) << std::setfill('0') << seconds << ".";
            oss << std::setw(3) << std::setfill('0') << milliseconds;
        }

        if (hasUnlimitedIterations())
        {
            oss << " (" << getIteration() << " / -)";
        }
        else
        {
            oss << " (" << getIteration() << " / " << getIterations() << ")";
        }

        m_font->draw(context, oss.str(), getFontColor());
    }
#endif
}
#endif

/* ************************************************************************ */

ViewPtr<PluginApi> Simulation::requirePlugin(const String& name)
{
    // Load plugin
    auto api = loadPlugin(name);

    if (api)
        return api;

    throw InvalidArgumentException("Plugin '" + name + "' not found");
}

/* ************************************************************************ */

ViewPtr<PluginApi> Simulation::loadPlugin(const String& name) noexcept
{
    try
    {
        // Test if plugin is used
        auto it = m_plugins.find(name);

        // Found
        if (it != m_plugins.end())
            return it->second;

        // Load plugin
        ViewPtr<PluginApi> api = PluginManager::s().load(name);

        if (!api)
            return nullptr;

        // Register API
        m_plugins.emplace(name, api);

        // Plugin loaded
        invoke(&SimulationListener::onPluginLoad, *this, name);

        // Init simulation
        api->initSimulation(*this);

        return api;
    }
    catch (const Exception& e)
    {
        Log::warning(e.what());
    }

    return nullptr;
}

/* ************************************************************************ */

void Simulation::storeDataTables()
{
    for (const auto& item : m_dataTables)
    {
        const auto& table = item.second;

        std::ofstream file(item.first + ".csv");

        Log::info("Saving data table '", item.first, "' into '", item.first, ".csv'");

        // Write headers
        writeCsvLine(file, table.getColumns());

        for (const auto& row : table.getRows())
            writeCsvLine(file, row);

        Log::info("Data table '", item.first, "' saved.");
    }
}

/* ************************************************************************ */

ViewPtr<Object> Simulation::query(const PositionVector& position) const noexcept
{
#ifdef ENABLE_PHYSICS
    const auto pos = b2Vec2(position.getX().value(), position.getY().value());
    QueryCallback query(pos);

    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
    aabb.lowerBound = pos - d;
    aabb.upperBound = pos + d;

    m_world.QueryAABB(&query, aabb);

    // Find object
    for (const auto& object : m_objects)
    {
        if (object->getBody() == query.getObject())
            return object.ptr;
    }
#endif

    return nullptr;
}

/* ************************************************************************ */

void Simulation::updateModules(units::Time dt)
{
    auto _ = measure_time("sim.modules", TimeMeasurementIterationOutput(this));

    // Sort modules by priority. Cannot be precomputed, because priority can change in previous iteration
    std::sort(m_modules.begin(), m_modules.end(),
        [](const ModuleContainer::ValueType& lhs, const ModuleContainer::ValueType& rhs) {
            return lhs.second->getPriority() > rhs.second->getPriority();
    });

    for (auto& module : m_modules)
        module.second->update(*this, dt);
}

/* ************************************************************************ */

void Simulation::updateObjects(units::Time dt)
{
    auto _ = measure_time("sim.objects", TimeMeasurementIterationOutput(this));

    // Update simulations objects
    // Can't use range-for because update can add a new object.
    for (ObjectContainer::SizeType i = 0u; i < m_objects.getCount(); ++i)
    {
        auto obj = m_objects[i];

        Assert(obj);
        obj->update(dt);
    }
}

/* ************************************************************************ */

void Simulation::detectDeserters()
{
    const auto hh = getWorldSize() * 0.5f;

    // Kill objects that are outside world
    for (auto& obj : m_objects)
    {
        // Ignore static objects
        if (obj->getType() == Object::Type::Static)
            continue;

        // Get object position
        const auto& pos = obj->getPosition();

        // Object is not in scene
        if (!pos.inRange(-hh, hh))
            m_objects.deleteObject(obj);
    };
}

/* ************************************************************************ */

void Simulation::deleteObjects()
{
    auto _ = measure_time("sim.delete", TimeMeasurementIterationOutput(this));

    // Remove deleted objects
    m_objects.removeDeleted();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
