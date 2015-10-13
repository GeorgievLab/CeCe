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
#include "simulator/Simulation.hpp"

// C++
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>

// Simulator
#include "core/Real.hpp"
#include "core/Log.hpp"
#include "core/Tuple.hpp"
#include "core/Exception.hpp"
#include "core/OutStream.hpp"
#include "simulator/Simulator.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/PluginManager.hpp"
#include "simulator/Obstacle.hpp"

#if CONFIG_RENDER_TEXT_ENABLE
#include "simulator/font.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

namespace {

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

bool Simulation::hasModule(const String& name) const noexcept
{
    auto it = std::find_if(m_modules.begin(), m_modules.end(), [&name](const Pair<String, UniquePtr<Module>>& p) {
        return p.first == name;
    });

    return it != m_modules.end();
}

/* ************************************************************************ */

Module* Simulation::getModule(const String& name) noexcept
{
    auto it = std::find_if(m_modules.begin(), m_modules.end(), [&name](const Pair<String, UniquePtr<Module>>& p) {
        return p.first == name;
    });

    if (it == m_modules.end())
        return nullptr;

    return it->second.get();
}

/* ************************************************************************ */

unsigned long Simulation::getObjectCountType(const String& className) const noexcept
{
    unsigned long res = 0ul;

    for (const auto& obj : getObjects())
    {
        if (obj->getClassName() == className)
            ++res;
    }

    return res;
}

/* ************************************************************************ */

Module* Simulation::addModule(String name, UniquePtr<Module> module)
{
    assert(module);
    m_modules.emplace_back(std::move(name), std::move(module));
    return m_modules.back().second.get();
}

/* ************************************************************************ */

Module* Simulation::useModule(const String& path, String storePath)
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
    for (auto& obj : getObjects())
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
            module->configure(moduleConfig, *this);
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
    for (auto& module : getModules())
        modules.push_back(module.second);

    // Sort modules by rendering order
    std::sort(modules.begin(), modules.end(), [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
        return lhs->getZOrder() < rhs->getZOrder();
    });

    // Render modules
    for (auto& module : modules)
        module->draw(context, *this);

    // Draw objects
    for (const auto& obj : getObjects())
    {
        assert(obj);
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

void Simulation::updateModules(units::Time dt)
{
    auto _ = measure_time("sim.modules", TimeMeasurementIterationOutput(this));

    // Sort modules by priority. Cannot be precomputed, because priority can change in previous iteration
    std::sort(m_modules.begin(), m_modules.end(),
        [](const ModuleContainer::value_type& lhs, const ModuleContainer::value_type& rhs) {
            return lhs.second->getPriority() > rhs.second->getPriority();
    });

    for (auto& module : m_modules)
        module.second->update(dt, *this);
}

/* ************************************************************************ */

void Simulation::updateObjects(units::Time dt)
{
    auto _ = measure_time("sim.objects", TimeMeasurementIterationOutput(this));

    // Update simulations objects
    //for (auto& obj : getObjects())
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        auto& obj = m_objects[i];
        assert(obj);
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
            m_objectsToDelete.push_back(obj.get());
    };
}

/* ************************************************************************ */

void Simulation::deleteObjects()
{
    auto _ = measure_time("sim.delete", TimeMeasurementIterationOutput(this));

    // Delete queued objects
    for (auto obj : m_objectsToDelete)
    {
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [obj](const ObjectContainer::value_type& rhs) {
            return obj.get() == rhs.get();
        }), m_objects.end());
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */
