
#pragma once

/* ************************************************************************ */

// C++
#include <string>

// V8
#include <v8.h>

// Simulator
#include "simulator/World.hpp"

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


// Public Mutators
public:

#ifdef ENABLE_RENDER

    /**
     * @brief Set render context.
     *
     * @param context
     */
    void setContext(render::Context& context) override;

#endif

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

#ifdef ENABLE_RENDER
    render::Context* m_renderContext = nullptr;
#endif

    /// V8 isolate
    v8::Isolate* m_isolate = nullptr;

    /// V8 run context.
    v8::Persistent<v8::Context> m_context;

    /// Output console object.
    v8::Persistent<v8::Object> m_console;

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
