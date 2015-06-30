/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/VectorUnits.hpp"
#include "core/UniquePtr.hpp"
#include "render/Camera.hpp"
#include "render/Program.hpp"
#include "render/ImageData.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Buffer;
class Color;
class Object;
class VertexFormat;
class Texture;

/* ************************************************************************ */

/**
 * @brief Primitive types for rendering.
 */
enum class PrimitiveType
{
    Lines,
    Triangles,
    Quads,
    TriangleFan
};

/* ************************************************************************ */

/**
 * @brief Renderer context.
 */
class DLL_EXPORT Context
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
    bool isInitialized() NOEXCEPT
    {
        return m_isInitialized;
    }


    /**
     * @brief Returns context camera.
     *
     * @return
     */
    Camera& getCamera() NOEXCEPT
    {
        return m_camera;
    }


    /**
     * @brief Get current image data.
     *
     * @return
     */
    ImageData getData() const NOEXCEPT;


    /**
     * @brief Returns if wireframe mode is enabled.
     *
     * @return
     */
    bool isWireframe() const NOEXCEPT;


// Public Mutators
public:


    /**
     * @brief Enable or disable wireframe mode.
     *
     * @param flag
     */
    void setWireframe(bool flag) NOEXCEPT;


// Public Operations
public:


    /**
     * @brief Initialize context.
     */
    void init() NOEXCEPT;


    /**
     * @brief Setup camera view.
     *
     * @param width  View width.
     * @param height View height.
     */
    void setView(int width, int height) NOEXCEPT;


    /**
     * @brief Setup stencil buffer test.
     *
     * @param width  View width.
     * @param height View height.
     */
    void setStencilBuffer(float width, float height) NOEXCEPT;


    /**
     * @brief Begin frame rendering.
     *
     * @param width  View width.
     * @param height View height.
     */
    void frameBegin(int width, int height) NOEXCEPT;


    /**
     * @brief End frame rendering.
     */
    void frameEnd() NOEXCEPT;


    /**
     * @brief Set matrix projecton mode.
     */
    void matrixProjection() NOEXCEPT;


    /**
     * @brief Set matrix view mode.
     */
    void matrixView() NOEXCEPT;


    /**
     * @brief Set matrix texture mode.
     */
    void matrixTexture() NOEXCEPT;


    /**
     * @brief Push transformation matrix.
     */
    void matrixPush() NOEXCEPT;


    /**
     * @brief Pop transformation matrix.
     */
    void matrixPop() NOEXCEPT;


    /**
     * @brief Set transformation matrix to identity.
     */
    void matrixIdentity() NOEXCEPT;


    /**
     * @brief Translate transformation matrix.
     *
     * @param pos Translation vector.
     */
    void matrixTranslate(const PositionVector& pos) NOEXCEPT;


    /**
     * @brief Scale transformation matrix.
     *
     * @param scale Scale vector.
     */
    void matrixScale(const ScaleVector& scale) NOEXCEPT;


    /**
     * @brief Scale transformation matrix.
     *
     * @param scale Scale number.
     */
    void matrixScale(float scale) NOEXCEPT
    {
        return matrixScale(ScaleVector{scale});
    }


    /**
     * @brief Rotate transformation matrix.
     *
     * @param angle Angle of the rotation.
     */
    void matrixRotate(units::Angle angle) NOEXCEPT;


    /**
     * @brief Set current color.
     *
     * @param color
     */
    void setColor(const Color& color) NOEXCEPT;


    /**
     * @brief Set vertex buffer.
     *
     * @param buffer A pointer to used buffer. Can be nullptr to unbind buffer.
     */
    void setVertexBuffer(Buffer* buffer) NOEXCEPT;


    /**
    * @brief Set vertex buffer format.
    *
    * @param format A pointer to vertex buffer format.
    */
    void setVertexFormat(const VertexFormat* format) NOEXCEPT;


    /**
    * @brief Set texture.
    *
    * @param texture Texture.
    */
    void setTexture(Texture* texture) NOEXCEPT;


    /**
    * @brief Set shader program.
    *
    * @param program A pointer to shader program.
    */
    void setProgram(Program* program) NOEXCEPT;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, bool value) NOEXCEPT;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, int value) NOEXCEPT;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, float value) NOEXCEPT;


    /**
    * @brief Set shader program parameter.
    *
    * @param id     Parameter ID.
    * @param value1 Parameter value.
    * @param value2 Parameter value.
    */
    void setProgramParam(Program::UniformId id, int value1, int value2) NOEXCEPT;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param color Color value.
    */
    void setProgramParam(Program::UniformId id, const render::Color& color) NOEXCEPT;


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

    /// If context is initialized.
    bool m_isInitialized = false;

    /// Camera.
    Camera m_camera;

    /// Rendering objects.
    DynamicArray<UniquePtr<Object>> m_objects;

    /// Rendering objects that going to be deleted.
    DynamicArray<UniquePtr<Object>> m_releasedObjects;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
