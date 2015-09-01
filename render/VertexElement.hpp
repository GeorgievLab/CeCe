/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
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
class VertexElement
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
    constexpr VertexElement(VertexElementType type, DataType dataType, unsigned int count)
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
    constexpr VertexElementType getType() const noexcept
    {
        return m_type;
    }


    /**
     * @brief Returns value data type.
     *
     * @return
     */
    constexpr DataType getDataType() const noexcept
    {
        return m_dataType;
    }


    /**
     * @brief Returns number of values.
     *
     * @return
     */
    constexpr unsigned int getCount() const noexcept
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
    constexpr size_t getSize() const noexcept
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
