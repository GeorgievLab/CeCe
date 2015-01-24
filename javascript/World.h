
#pragma once

/* ************************************************************************ */

// C++
#include <string>

// V8
#include <v8.h>

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

/**
 * @brief Javascript world.
 */
class World : public simulator::World
{
public:


    /**
     * @brief Constructor.
     */
    World() noexcept;


    /**
     * @brief Destructor.
     */
    ~World();


// Public Accessors
public:


    /**
     * @brief Returns V8 context.
     *
     * @return
     */
    v8::Persistent<v8::Context>& GetContext() noexcept
    {
        return m_context;
    }


    /**
     * @brief Returns V8 context.
     *
     * @return
     */
    const v8::Persistent<v8::Context>& GetContext() const noexcept
    {
        return m_context;
    }


// Public Operations
public:


    /**
     * @brief Reset world.
     */
    void reset() override;


    /**
     * @brief Load world code.
     *
     * @param source
     */
    void load(std::string source) override;


    /**
     * @brief Update world.
     *
     * @param step
     */
    void update(float step) noexcept override;


// Protected Operations
public:


    /**
     * @brief Init context.
     */
    void initContext();


    /**
     * @brief Run compiled script.
     *
     * @param script
     */
    void runScript(v8::Handle<v8::Script> script);


// Private Data Members
private:

    /// V8 isolate
    v8::Isolate* m_isolate;

    /// V8 run context.
    v8::Persistent<v8::Context> m_context;

    /// Script source.
    std::string m_source;

};

/* ************************************************************************ */

/**
 * @brief Returns a pointer to world instance.
 */
World* get_world();

/* ************************************************************************ */

}

/* ************************************************************************ */
