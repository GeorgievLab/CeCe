
#ifndef _JAVASCRIPT_WORLD_H_
#define _JAVASCRIPT_WORLD_H_

/* ************************************************************************ */

// V8
#include <v8.h>

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

/**
 * @brief World data.
 */
class WorldImplData : public simulator::WorldImplData
{
public:


    /**
     * @brief Constructor.
     */
    WorldImplData() noexcept;


    /**
     * @brief Destructor.
     */
    ~WorldImplData();


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


// Private Data Members
private:

    /// V8 run context.
    v8::Persistent<v8::Context> m_context;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _JAVASCRIPT_WORLD_H_
