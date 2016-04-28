/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include <iterator>
#include <iomanip>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/Tuple.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/object/FactoryManager.hpp"
#include "cece/plugin/Library.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/config/Exception.hpp"
#include "cece/module/FactoryManager.hpp"
#include "cece/simulator/TimeMeasurement.hpp"

#ifdef CECE_ENABLE_BOX2D_PHYSICS
#include "cece/simulator/ConverterBox2D.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#ifdef CECE_ENABLE_BOX2D_PHYSICS
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

Simulation::Simulation(plugin::Context& context, FilePath path) noexcept
    : m_pluginContext(context)
    , m_fileName(std::move(path))
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    , m_world{makeUnique<b2World>(b2Vec2{0.0f, 0.0f})}
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
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    return ConverterBox2D::getInstance().convertLinearAcceleration(m_world->GetGravity());
#else
    return AccelerationVector{Zero};
#endif
}

/* ************************************************************************ */

UniquePtr<InOutStream> Simulation::getResource(const String& name) noexcept
{
    // Path to resource
    const auto path = getFileName().parent_path() / name;

    if (!fileExists(path))
        return nullptr;

    auto file = makeUnique<FileStream>(path.string(), std::ios::in | std::ios::out | std::ios::binary);

    if (!file->is_open())
        return nullptr;

    return UniquePtr<InOutStream>{file.release()};
}

/* ************************************************************************ */

void Simulation::setTimeStep(units::Time dt)
{
    if (dt == Zero)
        throw InvalidArgumentException("Time step cannot be zero");

    m_timeStep = dt;

#ifdef CECE_ENABLE_BOX2D_PHYSICS
    ConverterBox2D::getInstance().setTimeStep(dt);
#endif
}

/* ************************************************************************ */

ViewPtr<const object::Type> Simulation::findObjectType(StringView name) const noexcept
{
    return m_objectClasses.get(name);
}

/* ************************************************************************ */

units::Time Simulation::getPhysicsEngineTimeStep() const noexcept
{
    return ConverterBox2D::getInstance().getTimeStepBox2D();
}

/* ************************************************************************ */

units::Length Simulation::getMaxObjectTranslation() const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    return ConverterBox2D::getInstance().getMaxObjectTranslation();
#else
    return units::Length{1e3};
#endif
}

/* ************************************************************************ */

void Simulation::setGravity(const AccelerationVector& gravity) noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    m_world->SetGravity(ConverterBox2D::getInstance().convertLinearAcceleration(gravity));
#else
    // TODO: store
#endif
}

/* ************************************************************************ */

ViewPtr<module::Module> Simulation::useModule(const String& nameSrc, String storePath)
{
    String name = nameSrc;

    // Find ':' in name - old definition
    auto pos = nameSrc.find(':');

    if (pos != String::npos)
    {
        name = nameSrc.substr(0, pos);
        storePath = nameSrc.substr(pos + 1);
    }

    if (storePath.empty())
        storePath = name;

    // Module exists, return the existing one
    if (hasModule(storePath))
        return getModule(storePath);

    Log::debug("Loading library: ", name);

    // Load only library
    Log::debug("Create module '", name, "'");

    // Create module with given name
    auto module = getPluginContext().createModule(name, *this);

    // Register module
    if (module)
    {
        if (storePath != name)
        {
            Log::info("Using module '", name, "' as '", storePath, "'");
        }
        else
        {
            Log::info("Using module '", name, "'");
        }

        return addModule(storePath, std::move(module));
    }

    Log::warning("Unable to create module '", name, "' (unsupported by library?)");

    return nullptr;
}

/* ************************************************************************ */

object::Object* Simulation::buildObject(const String& name, object::Object::Type type)
{
    // Try to find object internal object type
    auto desc = findObjectType(name);

    if (desc)
    {
        // Create parent object
        auto obj = buildObject(desc->baseName);

        // Configure
        obj->configure(desc->config, *this);

        return obj;
    }

    Log::debug("Create object '", name, "'");

    // Create object with given name
    auto object = getPluginContext().createObject(name, *this, type);

    // Register module
    if (object)
        return addObject(std::move(object));

    Log::warning("Unable to create object: ", name, " (unsupported by library?)");

    return nullptr;
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_BOX2D_PHYSICS
void Simulation::setPhysicsEngineTimeStep(units::Time dt) noexcept
{
    ConverterBox2D::getInstance().setTimeStepBox2D(dt);
}
#endif

/* ************************************************************************ */

UniquePtr<program::Program> Simulation::buildProgram(StringView name)
{
    Log::debug("Create program '", name, "'");

    // Create a program
    auto program = getPluginContext().createProgram(name);

    if (program)
        return program;

    Log::warning("Unable to create program '", name, "'");

    return nullptr;
}

/* ************************************************************************ */

void Simulation::reset()
{
    m_iteration = 0;
    m_totalTime = Zero;
    m_initialized = false;
}

/* ************************************************************************ */

bool Simulation::update()
{
    // Initialize simulation
    if (!isInitialized())
        throw RuntimeException("Simulation is not initialized");

    // Increase step number
    m_iteration++;
    m_totalTime += getTimeStep();

    // Clear all stored forces
    for (auto& obj : m_objects)
        obj->setForce(Zero);

    // Update modules
    updateModules();

    // Update objects
    updateObjects();

    // Detect object that leaved the scene
    detectDeserters();

    // Delete unused objects
    deleteObjects();

    // Store data
    if (m_dataOutObjects)
    {
        for (const auto& object : m_objects)
        {
            const auto pos = object->getPosition();
            const auto vel = object->getVelocity();

            *m_dataOutObjects <<
                // iteration
                getIteration() << ";" <<
                // totalTime
                getTotalTime() << ";" <<
                // id
                object->getId() << ";" <<
                // typeName
                object->getTypeName() << ";" <<
                // posX
                pos.getX() << ";" <<
                // posY
                pos.getY() << ";" <<
                // velX
                vel.getX() << ";" <<
                // velY
                vel.getY() << "\n"
            ;
        }
    }

#ifdef CECE_ENABLE_BOX2D_PHYSICS
    {
        auto _ = measure_time("sim.physics", TimeMeasurement(this));

        m_world->Step(getPhysicsEngineTimeStep().value(), 10, 10);
    }
#endif

    return (hasUnlimitedIterations() || getIteration() <= getIterations());
}

/* ************************************************************************ */

void Simulation::initialize(AtomicBool& termFlag)
{
    Assert(!isInitialized());

    // Initialize simulation
    m_initializers.init(*this);

    // Initialize modules
    m_modules.init(termFlag);

    m_initialized = true;
}

/* ************************************************************************ */

void Simulation::loadConfig(const config::Configuration& config)
{
    setWorldSize(config.get<SizeVector>("world-size"));
    setTimeStep(config.get<units::Time>("dt"));

    if (config.has("length-coefficient"))
        ConverterBox2D::getInstance().setLengthCoefficient(config.get<RealType>("length-coefficient"));

    setGravity(config.get("gravity", getGravity()));
    setIterations(config.get("iterations", getIterations()));

#ifdef CECE_ENABLE_RENDER
    setVisualized(config.get("visualized", isVisualized()));
    setBackgroundColor(config.get("background", getBackgroundColor()));
#endif

    // Parse plugins
    for (auto&& pluginConfig : config.getConfigurations("plugin"))
    {
        // Returns valid pointer or throws an exception
        requirePlugin(pluginConfig.get("name"))->loadConfig(*this, pluginConfig);
    }

    // Parse parameters
    for (auto&& parameterConfig : config.getConfigurations("parameter"))
    {
        // Get parameter name
        const auto name = parameterConfig.get("name");

        // Do not override previously defined parameters
        if (hasParameters(name))
            continue;

        if (!parameterConfig.has("value"))
            throw InvalidArgumentException("Missing parameter value. Define "
                "it in simulation file or as application argument");

        // Store new parameter
        setParameter(name, parameterConfig.get("value"));
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
        const String typeName = initConfig.has("language")
            ? initConfig.get("language")
            : initConfig.get("type");

        auto initializer = getPluginContext().createInitializer(typeName);

        if (initializer)
        {
            // Configure initializer
            initializer->loadConfig(*this, initConfig);

            // Register initializer
            addInitializer(std::move(initializer));
        }
    }

    // Parse modules
    for (auto&& moduleConfig : config.getConfigurations("module"))
    {
        // Get name
        auto name = moduleConfig.get("name");

        ViewPtr<module::Module> module;

        if (!hasModule(name))
        {
            const String typeName = moduleConfig.has("language")
                ? moduleConfig.get("language")
                : moduleConfig.has("type")
                    ? moduleConfig.get("type")
                    : name
            ;

            auto mod = getPluginContext().createModule(typeName, *this);

            module = addModule(std::move(name), std::move(mod));
        }
        else
        {
            module = getModule(name);
        }

        // Configure module
        if (module)
            module->loadConfig(moduleConfig);
    }

    // Parse programs
    for (auto&& programConfig : config.getConfigurations("program"))
    {
        const String typeName = programConfig.has("language")
            ? programConfig.get("language")
            : programConfig.get("type");

        auto program = getPluginContext().createProgram(typeName);

        if (program)
        {
            // Configure program
            program->loadConfig(*this, programConfig);

            // Register program
            addProgram(programConfig.get("name"), std::move(program));
        }
    }

    // Parse objects
    for (auto&& objectConfig : config.getConfigurations("object"))
    {
        // Create object
        auto object = buildObject(
            objectConfig.get("class"),
            objectConfig.get("type", object::Object::Type::Dynamic)
        );

        if (object)
            object->configure(objectConfig, *this);
    }

    if (config.has("data-out-objects-filename"))
    {
        m_dataOutObjects = makeUnique<OutFileStream>(config.get("data-out-objects-filename"));
        *m_dataOutObjects << "iteration;totalTime;id;typeName;posX;posY;velX;velY\n";
    }
}

/* ************************************************************************ */

void Simulation::storeConfig(config::Configuration& config) const
{
    config.set("world-size", getWorldSize());
    config.set("dt", getTimeStep());
    config.set("length-coefficient", ConverterBox2D::getInstance().getLengthCoefficient());
    config.set("gravity", getGravity());
    config.set("iterations", getIterations());

#ifdef CECE_ENABLE_RENDER
    config.set("background", getBackgroundColor());
    config.set("visualized", isVisualized());
#endif

    // Store parameters
    for (const auto& parameter : getParameters())
    {
        auto parameterConfig = config.addConfiguration("parameter");
        parameterConfig.set("name", parameter.name);
        parameterConfig.set("value", parameter.value);
    }

    // Store plugins
    for (const auto& plugin : m_plugins)
    {
        auto pluginConfig = config.addConfiguration("plugin");
        pluginConfig.set("name", plugin.first);
        plugin.second->storeConfig(*this, pluginConfig);
    }

    // Store object types
    for (const auto& type : m_objectClasses)
    {
        auto typeConfig = config.addConfiguration("type");
        typeConfig.set("name", type.name);
        typeConfig.set("base", type.baseName);
        // TODO: rest ot configuration
    }

    // Store initializers
    for (const auto& init : m_initializers)
    {
        // TODO: improve
        auto initConfig = config.addConfiguration("init");
        init->storeConfig(*this, initConfig);
    }

    // Store modules
    for (const auto& module : m_modules)
    {
        auto moduleConfig = config.addConfiguration("module");
        moduleConfig.set("name", module.name);
        module->storeConfig(moduleConfig);
    }

    // Store programs
    for (const auto& program : m_programs)
    {
        auto programConfig = config.addConfiguration("program");
        programConfig.set("name", program.name);
        program->storeConfig(*this, programConfig);
    }

    // TODO: store objects
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Simulation::draw(render::Context& context)
{
    context.setStencilBuffer(getWorldSize().getWidth().value(), getWorldSize().getHeight().value());

    // Render modules
    m_modules.draw(context);

    // Draw objects
    for (auto& obj : m_objects)
    {
        Assert(obj);
        if (obj->isVisible())
            obj->draw(context);
    }

#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)
    if (isDrawPhysics())
        m_world->DrawDebugData();
#endif

}
#endif

/* ************************************************************************ */

ViewPtr<plugin::Api> Simulation::requirePlugin(const String& name)
{
    // Load plugin
    auto api = loadPlugin(name);

    if (api)
        return api;

    throw InvalidArgumentException("Plugin '" + name + "' not found");
}

/* ************************************************************************ */

ViewPtr<plugin::Api> Simulation::loadPlugin(const String& name) noexcept
{
    try
    {
        // Test if plugin is used
        auto it = m_plugins.find(name);

        // Found
        if (it != m_plugins.end())
            return it->second;

        // Load plugin
        auto api = plugin::Manager::s().load(name);

        if (!api)
            return nullptr;

        // Register API
        m_plugins.emplace(name, api);

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

ViewPtr<object::Object> Simulation::query(const PositionVector& position) const noexcept
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    const auto pos = ConverterBox2D::getInstance().convertPosition(position);
    QueryCallback query(pos);

    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
    aabb.lowerBound = pos - d;
    aabb.upperBound = pos + d;

    m_world->QueryAABB(&query, aabb);

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

void Simulation::updateModules()
{
    auto _ = measure_time("sim.modules", TimeMeasurement(this));
    m_modules.update();
}

/* ************************************************************************ */

void Simulation::updateObjects()
{
    auto _ = measure_time("sim.objects", TimeMeasurement(this));

    // Update simulations objects
    // Can't use range-for because update can add a new object.
    for (object::Container::SizeType i = 0u; i < m_objects.getCount(); ++i)
    {
        auto obj = m_objects[i];

        Assert(obj);
        obj->update(getTimeStep());
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
        if (obj->getType() == object::Object::Type::Static)
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
    auto _ = measure_time("sim.delete", TimeMeasurement(this));

    // Remove deleted objects
    m_objects.removeDeleted();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
