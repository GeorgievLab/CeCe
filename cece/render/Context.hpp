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

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

#ifndef CECE_ENABLE_RENDER
#error Rendering part requires enabled rendering.
#endif

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/render/Camera.hpp"
#include "cece/render/Program.hpp"
#include "cece/render/ImageData.hpp"
#include "cece/render/Color.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Buffer;
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
class CECE_EXPORT Context
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
    bool isInitialized() const noexcept
    {
        return m_isInitialized;
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
     * @brief Returns buffer size.
     *
     * @return
     */
    Size getSize() const noexcept;


    /**
     * @brief Get current image data.
     *
     * @param data  Output buffer pointer. Expected size is
     * `4 * sizeof(uchar8_t) * getSize().getWidth() * getSize().getHeight()`.
     * @param alpha If result data should contains alpha channel.
     * @param bgra  If pixels should be in BGRA instead of RGBA.
     */
    void getData(void* data, bool alpha = false, bool bgra = false) const noexcept;


    /**
     * @brief Get current image data.
     *
     * @param alpha If result data should contains alpha channel.
     * @param bgra  If pixels should be in BGRA instead of RGBA.
     *
     * @return
     */
    ImageData getData(bool alpha = false, bool bgra = false) const noexcept;


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
     *
     * @param clearColor
     */
    void init(const render::Color& clearColor = render::colors::WHITE) noexcept;


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
     * @brief Disable stencil buffer.
     */
    void disableStencilBuffer() noexcept;


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
     * @brief Enable alpha.
     */
    void enableAlpha() noexcept;


    /**
     * @brief Disable alpha.
     */
    void disableAlpha() noexcept;


    /**
     * @brief Push color attributes.
     */
    void colorPush() noexcept;


    /**
     * @brief Pop color attributes.
     */
    void colorPop() noexcept;


    /**
     * @brief Set matrix projecton mode.
     */
    void matrixProjection() noexcept;


    /**
     * @brief Set matrix view mode.
     */
    void matrixView() noexcept;


    /**
     * @brief Set matrix texture mode.
     */
    void matrixTexture() noexcept;


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
    void matrixScale(const ScaleVector& scale) noexcept;


    /**
     * @brief Scale transformation matrix.
     *
     * @param scale Scale number.
     */
    void matrixScale(RealType scale) noexcept
    {
        return matrixScale(ScaleVector::createSingle(scale));
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
    * @brief Set vertex buffer format.
    *
    * @param format A pointer to vertex buffer format.
    */
    void setVertexFormat(const VertexFormat* format) noexcept;


    /**
    * @brief Set texture.
    *
    * @param texture Texture.
    */
    void setTexture(Texture* texture) noexcept;


    /**
    * @brief Set shader program.
    *
    * @param program A pointer to shader program.
    */
    void setProgram(Program* program) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, bool value) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, int value) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, unsigned int value) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, float value) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, double value) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param value Parameter value.
    */
    void setProgramParam(Program::UniformId id, long double value) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id     Parameter ID.
    * @param value1 Parameter value.
    * @param value2 Parameter value.
    */
    void setProgramParam(Program::UniformId id, int value1, int value2) noexcept;


    /**
    * @brief Set shader program parameter.
    *
    * @param id    Parameter ID.
    * @param color Color value.
    */
    void setProgramParam(Program::UniformId id, const render::Color& color) noexcept;


    /**
     * @brief Draw primitives from current buffer.
     *
     * @param type   Primitive type.
     * @param count  Number of drawn primitives
     * @param offset Offset from vertex buffer.
     */
    void draw(PrimitiveType type, unsigned int count, unsigned int offset = 0u);


    /**
     * @brief Draw single line.
     *
     * @param pos   Line position.
     * @param size  Line size.
     * @param color Line color.
     */
    void drawLine(Vector<float> pos, Vector<float> size, const Color& color = colors::WHITE);


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
}

/* ************************************************************************ */
