#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

/* ************************************************************************ */

// C++
#include <memory>

// wxWidget
#include <wx/scopedptr.h>
#include <wx/thread.h>
#include <wx/event.h>

// Core
#include "World.h"

/* ************************************************************************ */

wxDECLARE_EVENT(EVT_SIMULATION_UPDATE, wxThreadEvent);

/* ************************************************************************ */

/**
 * @brief World simulator.
 */
class Simulator : public wxThreadHelper
{
public:


    /**
     * @brief Constructor.
     *
     * @param handler Event handler.
     */
    explicit Simulator(wxEvtHandler* handler)
        : m_handler(handler)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    ~Simulator()
    {
        Stop();
    }


public:


    /**
     * @brief If simulation is running.
     *
     * @return
     */
    bool IsRunning() const noexcept
    {
        return GetThread() && GetThread()->IsRunning();
    }


    /**
     * @brief Returns current world.
     *
     * @return
     */
    World* GetWorld() const noexcept
    {
        return m_world.get();
    }


public:


    /**
     * @brief Set new current world.
     *
     * @param world
     */
    void SetWorld(std::unique_ptr<World> world) noexcept
    {
        m_world = std::move(world);
    }


public:


    /**
     * @brief Start simulation.
     */
    void Start();


    /**
     * @brief Stop simulation.
     */
    void Stop();


    /**
     * @brief Perform one simulation step.
     */
    void Step();


protected:


    /**
     * @brief Background thread code.
     *
     * @return
     */
    wxThread::ExitCode Entry() override;


private:


    /// Event handler for world update events.
    wxEvtHandler* m_handler;

    /// Simulated world.
    std::unique_ptr<World> m_world;

};

/* ************************************************************************ */

#endif // _SIMULATOR_H_
