
#ifndef _LIBRARY_JS_WORLD_H_
#define _LIBRARY_JS_WORLD_H_

/* ************************************************************************ */

// V8
#include <v8.h>

// Core
#include "World.h"

/* ************************************************************************ */

/**
 * @brief World for JavaScript (V8 engine)
 */
class JsWorld : public World
{
public:


    /**
     * @brief Constructor.
     */
    JsWorld() noexcept;


    /**
     * @brief Destructor.
     */
    ~JsWorld();


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


private:

    /// V8 run context.
    v8::Persistent<v8::Context> m_context;

};

/* ************************************************************************ */

#endif // _LIBRARY_JS_WORLD_LOADER_H_
