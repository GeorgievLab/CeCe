
#pragma once

/* ************************************************************************ */

// C++
#include <memory>

// Physics
#include "simulator/Cell.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public Cell
{

// Public Classes
public:


    /**
     * @brief Yeast bud class.
     */
    class Bud
    {

    // Public Ctors & Dtors
    public:


        /**
         * @brief Constructor.
         *
         * @param owner
         * @param theta
         * @param phi
         */
        explicit Bud(Yeast* owner, Angle theta, Angle phi)
            : m_owner(owner)
            , m_theta(theta)
            , m_phi(phi)
        {
            // Nothing to do
        }


    // Public Accessors
    public:


        /**
         * @brief Returns owner.
         *
         * @return
         */
        Yeast* getOwner() const noexcept
        {
            return m_owner;
        }


        /**
         * @brief Returns theta angle.
         *
         * @return
         */
        Angle getTheta() const noexcept
        {
            return m_theta;
        }


        /**
         * @brief Returns phi angle.
         *
         * @return
         */
        Angle getPhi() const noexcept
        {
            return m_phi;
        }


        /**
         * @brief Get bud volume.
         *
         * @return
         */
        Volume getVolume() const noexcept
        {
            return m_volume;
        }


    // Public Mutators
    public:


        /**
         * @brief Set bud volume.
         *
         * @param volume
         */
        void setVolume(Volume volume) noexcept
        {
            m_volume = volume;
        }


    // Private Data Members
    private:

        /// Owner.
        Yeast* m_owner;

        /// Bud position.
        Angle m_theta;

        /// Bud position.
        Angle m_phi;

        /// Cell volume.
        Volume m_volume = 1_um;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     */
    Yeast(World* world);


    /**
     * @brief Destructor.
     */
    virtual ~Yeast();


// Public Accessors
public:


    /**
     * @brief Returns yeast's bud.
     *
     * @return
     */
    Bud* getBud() const noexcept
    {
        return m_bud.get();
    }


// Public Operations
public:


    /**
     * @brief Update yeast state.
     */
    void update() override;


    /**
     * @brief Create new yeast bud.
     */
    void budCreate();


    /**
     * @brief Release bud cell.
     */
    void budRelease();


#ifdef ENABLE_RENDER

    /**
     * @brief Render cell.
     *
     * @param context
     */
    virtual void draw(render::Context& context);

#endif

// Private Data Members
private:

    /// Bud cell.
    std::unique_ptr<Bud> m_bud;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
