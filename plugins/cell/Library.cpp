/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Plugin
#include "Generator.hpp"
#include "Yeast.hpp"
#include "Cell.hpp"
#include "StoreMolecules.hpp"

/* ************************************************************************ */

using namespace simulator;
using namespace plugin::cell;

/* ************************************************************************ */

/**
 * @brief Cell plugin API.
 */
class CellApi : public PluginApi
{

    /**
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        if (name == "generator")
            return makeUnique<Generator>();

        return nullptr;
    }


    /**
     * @brief Create object from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param type       Object type.
     *
     * @return Created object.
     */
    UniquePtr<Object> createObject(Simulation& simulation, const String& name, Object::Type type = Object::Type::Dynamic) noexcept override
    {
        if (name == "Yeast")
            return makeUnique<Yeast>(simulation, type);
        else if (name == "Cell")
            return makeUnique<Cell>(simulation, type);

        return nullptr;
    }


    /**
     * @brief Create program from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Program name.
     * @param code       Optional program code.
     *
     * @return Created program.
     */
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        if (name == "store-molecules")
            return StoreMolecules{};

        return {};
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(cell, CellApi)

/* ************************************************************************ */
