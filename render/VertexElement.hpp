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
#include "render/datatype.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Types used for vertex format definition elements.
 */
enum class VertexElementType
{
    /// Position.
    Position,
    /// Color.
    Color,
    /// Normal.
    Normal,
    /// Texture coordinates.
    TexCoord
};

/* ************************************************************************ */

/**
 * @brief Vertex format element.
 */
class DLL_EXPORT VertexElement
{

// Public Ctors
public:


    /**
     * @brief Constructor
     *
     * @param type     Element type.
     * @param dataType Data type.
     * @param count    Number of values.
     */
    CONSTEXPR VertexElement(VertexElementType type, DataType dataType, unsigned int count)
        : m_type(type)
        , m_dataType(dataType)
        , m_count(count)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns element type.
     *
     * @return
     */
    CONSTEXPR VertexElementType getType() const NOEXCEPT
    {
        return m_type;
    }


    /**
     * @brief Returns value data type.
     *
     * @return
     */
    CONSTEXPR DataType getDataType() const NOEXCEPT
    {
        return m_dataType;
    }


    /**
     * @brief Returns number of values.
     *
     * @return
     */
    CONSTEXPR unsigned int getCount() const NOEXCEPT
    {
        return m_count;
    }


// Public Operations
public:


    /**
     * @brief Calculate element size.
     *
     * @return
     */
    CONSTEXPR size_t getSize() const NOEXCEPT
    {
        return getCount() * data_type_size(getDataType());
    }


// Private Data Members
private:


    /// Type of element.
    VertexElementType m_type;

    /// Component data type.
    DataType m_dataType;

    /// A number of components.
    unsigned int m_count;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
