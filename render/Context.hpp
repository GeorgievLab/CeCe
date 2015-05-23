
#pragma once

/* ************************************************************************ */

// C++
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

// Simulator
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "render/Camera.hpp"
#include "render/Position.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Buffer;
class Color;
class Object;

/* ************************************************************************ */

/**
 * @brief Primitive types for rendering.
 */
enum class PrimitiveType
{
    Lines,
    Triangles,
    Quads
};

/* ************************************************************************ */

/**
 * @brief Renderer context.
 */
class Context
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Context();


    /**
     * @brief Destructor.
     */
    ~Context();


// Public Accessors
public:


    /**
     * @brief Returns if context is initialized.
     *
     * @return
     */
    bool isInitialized() noexcept
    {
        return m_is_init;
    }


    /**
     * @brief Returns context camera.
     *
     * @return
     */
    Camera& getCamera() noexcept
    {
        return m_camera;
    }


    /**
     * @brief Get current image data.
     *
     * @return
     */
    std::pair<std::vector<std::uint8_t>, Vector<unsigned>> getData() const noexcept;


    /**
     * @brief Returns if wireframe mode is enabled.
     *
     * @return
     */
    bool isWireframe() const noexcept;


// Public Mutators
public:


    /**
     * @brief Enable or disable wireframe mode.
     *
     * @param flag
     */
    void setWireframe(bool flag) noexcept;


// Public Operations
public:


    /**
     * @brief Initialize context.
     */
    void init() noexcept;


    /**
     * @brief Setup camera view.
     *
     * @param width  View width.
     * @param height View height.
     */
    void setView(int width, int height) noexcept;


    /**
     * @brief Setup stencil buffer test.
     *
     * @param width  View width.
     * @param height View height.
     */
    void setStencilBuffer(float width, float height) noexcept;


    /**
     * @brief Begin frame rendering.
     *
     * @param width  View width.
     * @param height View height.
     */
    void frameBegin(int width, int height) noexcept;


    /**
     * @brief End frame rendering.
     */
    void frameEnd() noexcept;


    /**
     * @brief Push transformation matrix.
     */
    void matrixPush() noexcept;


    /**
     * @brief Pop transformation matrix.
     */
    void matrixPop() noexcept;


    /**
     * @brief Set transformation matrix to identity.
     */
    void matrixIdentity() noexcept;


    /**
     * @brief Translate transformation matrix.
     *
     * @param pos Translation vector.
     */
    void matrixTranslate(const PositionVector& pos) noexcept;


    /**
     * @brief Scale transformation matrix.
     *
     * @param scale Scale vector.
     */
    void matrixScale(const Vector<float>& scale) noexcept;


    /**
     * @brief Scale transformation matrix.
     *
     * @param scale Scale number.
     */
    void matrixScale(float scale) noexcept
    {
        return matrixScale(Vector<float>{scale});
    }


    /**
     * @brief Rotate transformation matrix.
     *
     * @param angle Angle of the rotation.
     */
    void matrixRotate(units::Angle angle) noexcept;


    /**
     * @brief Set current color.
     *
     * @param color
     */
    void setColor(const Color& color) noexcept;


    /**
     * @brief Set vertex buffer.
     *
     * @param buffer A pointer to used buffer. Can be nullptr to unbind buffer.
     */
    void setVertexBuffer(Buffer* buffer) noexcept;


    /**
     * @brief Draw primitives from current buffer.
     *
     * @param type   Primitive type.
     * @param count  Number of drawn primitives
     * @param offset Offset from vertex buffer.
     */
    void draw(PrimitiveType type, unsigned int count, unsigned int offset = 0u);


    /**
     * @brief Add rendering object.
     *
     * @param obj
     */
    Object* addObject(Object* obj);


    /**
     * @brief Create rendering object.
     *
     * @tparam T
     * @tparam Args
     *
     * @param
     */
    template<typename T, typename... Args>
    T* createObject(Args&&... args)
    {
        return static_cast<T*>(addObject(new T(*this, std::forward<Args>(args)...)));
    }


    /**
     * @brief Delete object.
     *
     * @param obj
     */
    void deleteObject(const Object* obj);


    /**
     * @brief Delete released objects.
     */
    void deleteReleasedObjects();


// Private Data Members
private:

    /// If context is init
    bool m_is_init = false;

    /// Camera.
    Camera m_camera;

    /// Rendering objects.
    std::vector<std::unique_ptr<Object>> m_objects;

    /// Rendering objects that going to be deleted.
    std::vector<std::unique_ptr<Object>> m_releasedObjects;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
