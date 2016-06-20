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

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/IteratorRange.hpp"
#include "cece/core/Assert.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/StringView.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Lexical exception for tokenizer.
 */
class LexicalException : public RuntimeException
{
    using RuntimeException::RuntimeException;
};

/* ************************************************************************ */

/**
 * @brief Basic token class.
 */
template<
    typename Code,
    typename Value = StringView,
    typename Position = unsigned long
>
struct BasicToken
{

// Public Types
public:


    /// The token code type.
    using CodeType = Code;

    /// The value type.
    using ValueType = Value;

    /// The position type.
    using PositionType = Position;


// Public Constants
public:


    /// Code for invalid token.
    static constexpr CodeType INVALID{};


// Public Data Members
public:


    /// Token code.
    CodeType code;

    /// Token value.
    ValueType value;

    /// Token starting position.
    PositionType position;


// Public Ctors & Dtors
public:


    /**
     * @brief Creates an invalid token.
     */
    BasicToken() noexcept
        : BasicToken{INVALID}
    {
        // Nothing to do
    }


    /**
     * @brief Creates a token for given token type.
     *
     * @param code Token code.
     */
    explicit BasicToken(CodeType code) noexcept
        : BasicToken{std::move(code), ValueType{}}
    {
        // Nothing to do
    }


    /**
     * @brief Creates a token for given type and value.
     *
     * @param code Token code.
     * @param value Token value.
     */
    BasicToken(CodeType code, ValueType value) noexcept
        : BasicToken{std::move(code), std::move(value), PositionType{}}
    {
        // Nothing to do
    }


    /**
     * @brief Creates a token for given type and value.
     *
     * @param code Token identifier.
     * @param value Token value.
     * @param position Token start position.
     */
    BasicToken(CodeType code, ValueType value, PositionType position) noexcept
        : code(std::move(code)), value(std::move(value)), position(std::move(position))
    {
        // Nothing to do
    }


// Operators
public:


    /**
     * @brief Converts the token into bool value.
     *
     * The value indicates if token is valid.
     *
     * @return Is token valid?
     */
    operator bool() const noexcept
    {
        return isValid();
    }


// Public Accessors
public:


    /**
     * @brief Checks if token is valid.
     *
     * @return Is token valid?
     */
    bool isValid() const noexcept
    {
        return (code != INVALID);
    }

};

/* ************************************************************************ */

/**
 * @brief Tokenizer iterator.
 *
 * Iterator used by tokenizer to fetch next token for each increment
 * operation. Iterator stores only current token and the previous tokens
 * are thrown away.
 *
 * @tparam Tokenizer Tokenizer type.
 */
template<typename Tokenizer>
class BasicTokenizerIterator
{
    template<typename, typename, typename, typename>
    friend class BasicTokenizer;


// Public Types
public:


    /// Iterator traits
    using iterator_category = std::input_iterator_tag;
    using value_type = typename Tokenizer::TokenType;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::add_const<value_type>::type*;
    using reference = typename std::add_const<value_type>::type&;


// Protected Ctors & Dtors
protected:


    /**
     * @brief Creates a pass the end iterator.
     */
    BasicTokenizerIterator() = default;


    /**
     * @brief Creates an iterator for given source range.
     *
     * @param tokenizer A pointer to source tokenizer.
     */
    BasicTokenizerIterator(Tokenizer * const tokenizer) noexcept
        : m_tokenizer{tokenizer}
    {
        // Fetch the first token
        fetchToken();
    }


// Public Operators
public:


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return Are iterators equal?
     *
     * @todo Better comparsion for different tokenizers.
     */
    bool operator==(const BasicTokenizerIterator& rhs) const noexcept
    {
        return (m_ok == rhs.m_ok);
    }


    /**
     * @brief Compares two iterators.
     *
     * @param rhs Second iterator.
     *
     * @return Aren't iterators equal?
     */
    bool operator!=(const BasicTokenizerIterator& rhs) const noexcept
    {
        return !operator==(rhs);
    }


    /**
     * @brief Dereference operator.
     *
     * @return A reference to the current token.
     */
    reference operator*() const noexcept
    {
        return m_token;
    }

    /**
     * @brief Dereference operator.
     *
     * @return A pointer to the current token.
     */
    pointer operator->() const noexcept
    {
        return &m_token;
    }


    /**
     * @brief Pre-increment operator.
     *
     * @return Reference to this.
     */
    BasicTokenizerIterator& operator++()
    {
        fetchToken();
        return *this;
    }


    /**
     * @brief Post-increment operator.
     *
     * @note Is deleted (because I'm to lazy to write it)
     */
    BasicTokenizerIterator operator++(int) = delete;


// Protected Operations
protected:


    /**
     * @brief Loads token from the tokenizer's source.
     */
    void fetchToken()
    {
        // Trying to fetch token by invalid iterator
        Assert(m_tokenizer != nullptr);

        // Nothing to read
        if (m_tokenizer->isEof())
        {
            // Make invalid token
            m_token = value_type{};
            m_ok = false;
        }
        else
        {
            // Read one token from input
            m_token = m_tokenizer->fetch();
            m_ok = true;
        }
    }


// Private Data Members
private:

    /// Pointer to tokenizer.
    Tokenizer * const m_tokenizer = nullptr;

    /// Current token.
    value_type m_token;

    /// If token is loaded
    bool m_ok = false;

};

/* ************************************************************************ */

/**
 * @brief Basic tokenizer class.
 *
 * @tparam Derived       Type of derived tokenizer.
 * @tparam Token         Type of token.
 * @tparam InputIterator Type of input iterator.
 * @tparam Source        Type of source range.
 */
template<
    typename Derived,
    typename Token,
    typename InputIterator,
    typename Source = IteratorRange<InputIterator>
>
class BasicTokenizer
{
    template<typename>
    friend class BasicTokenizerIterator;


    friend Derived;


// Public Types
public:


    /// The token type.
    using TokenType = Token;

    /// Output iterator type.
    using IteratorType = BasicTokenizerIterator<Derived>;

    /// Input value type.
    using ValueType = typename std::iterator_traits<InputIterator>::value_type;

    /// Source iterator range.
    using SourceRange = Source;

    // Input iterator type.
    using InputIteratorType = InputIterator;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param range Source range.
     */
    explicit BasicTokenizer(SourceRange range) noexcept
        : m_range{std::move(range)}
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param beg Begin iterator.
     * @param end End iterator.
     */
    BasicTokenizer(InputIterator beg, InputIterator end) noexcept
        : m_range{beg, end}
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @tparam Src Type of source range.
     *
     * @param source Source object.
     */
    template<typename Src>
    explicit BasicTokenizer(Src&& source) noexcept
        : m_range(makeRange(std::forward<Src>(source)))
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns tokenizer source range.
     *
     * @return
     */
    SourceRange& getRange() noexcept
    {
        return m_range;
    }


    /**
     * @brief Returns tokenizer source range.
     *
     * @return
     */
    const SourceRange& getRange() const noexcept
    {
        return m_range;
    }


    /**
     * @brief Check if source is empty.
     *
     * @return Is source empty?
     */
    bool isEof() const noexcept
    {
        return m_range.isEmpty();
    }


    /**
     * @brief Returns an iterator to first token.
     *
     * @return An iterator.
     */
    IteratorType begin() noexcept
    {
        return IteratorType{static_cast<Derived*>(this)};
    }


    /**
     * @brief Returns an iterator points past the end.
     *
     * @return An iterator.
     */
    IteratorType end() const noexcept
    {
        return IteratorType{};
    }


    /**
     * @brief Returns current value.
     *
     * @return
     */
    ValueType value() const noexcept
    {
        return m_range.front();
    }


    /**
     * @brief Test if current source value match the given value.
     *
     * This operation requires Value to be comparable with source value_type.
     *
     * @tparam Value Type of tested value.
     *
     * @param val Tested value.
     *
     * @return Result of the test.
     */
    template<typename Value>
    bool is(Value&& val) const noexcept
    {
        return (value() == val);
    }


    /**
     * @brief Test if current character match one of the given values.
     *
     * Its recommended to pass arguments sorted by propability of each value.
     *
     * @tparam Value Type of tested value.
     * @tparam Values Types of tested values.
     *
     * @param value First tested value.
     * @param values Tested values.
     *
     * @return Result of the test.
     */
    template<typename Value, typename... Values>
    bool is(Value&& value, Values&&... values) const noexcept
    {
        // Recursive testing.
        return
            is(std::forward<Value>(value)) ||
            is(std::forward<Values>(values)...)
        ;
    }


    /**
     * @brief Test if current character is in range of values.
     *
     * @tparam Value1 Type of tested value.
     * @tparam Value2 Type of tested value.
     *
     * @param val1 First value from range.
     * @param val2 Last value from range.
     *
     * @return Result of the test.
     */
    template<typename Value1, typename Value2>
    bool isRange(Value1&& val1, Value2&& val2) const noexcept
    {
        return (value() >= val1) && (value() <= val2);
    }


    /**
     * @brief Test if current character match to the given value.
     *
     * Difference between `match` function and `is` function is the `match`
     * function consumes input character if the character match requirements.
     * The `is` function doesn't change tokenizer state but the `match`
     * function does.
     *
     * @tparam Values Types of tested values.
     *
     * @param values Tested values.
     *
     * @return Result of the test.
     */
    template<typename... Values>
    bool match(Values&&... values)
    {
        if (is(std::forward<Values>(values)...))
        {
            // Move to the next character
            next();
            return true;
        }

        return false;
    }


    /**
     * @brief Skip given values.
     *
     * @tparam Values Types of tested values.
     *
     * @param values Tested values.
     *
     * @return !isEof().
     */
    template<typename... Values>
    bool skip(Values&&... values) noexcept
    {
        while (!isEof() && is(values...))
            next();

        return !isEof();
    }


    /**
     * @brief Try to find one of the given values.
     *
     * @tparam Values Types of tested values.
     *
     * @param values Tested values.
     *
     * @return !isEof().
     */
    template<typename... Values>
    bool find(Values&&... values) noexcept
    {
        while (!isEof() && !is(values...))
            next();

        return !isEof();
    }


// Protected Operations
protected:


    /**
     * @brief Read next value from input range.
     */
    void next() noexcept
    {
        m_range.advanceBegin();
    }


    /**
     * @brief Get one token from source.
     *
     * @note It calls Derived::tokenize()
     *
     * @return Result token.
     *
     * @throw LexicalError
     */
    TokenType fetch()
    {
        static_assert(std::is_member_function_pointer<decltype(&Derived::tokenize)>::value,
            "Function Derived::tokenize is required");

        // End of source -> return invalid token
        if (isEof())
            return TokenType{};

        // Call parent's member function
        return static_cast<Derived*>(this)->tokenize();
    }


    /**
     * @brief Write note message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     */
    template<typename Message>
    void note(Message&& msg) const noexcept
    {
        Log::info(msg);
    }


    /**
     * @brief Write warning message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     */
    template<typename Message>
    void warning(Message&& msg) const noexcept
    {
        Log::warning(msg);
    }


    /**
     * @brief Write error message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     */
    template<typename Message>
    void error(Message&& msg) const
    {
        Log::error(msg);
    }


    /**
     * @brief Write fatal error message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     *
     * @throw Exception
     */
    template<typename Message>
    [[noreturn]] void fatalError(Message&& msg) const
    {
        throw LexicalException(msg);
    }


// Private Data Members
private:


    /// Source range.
    SourceRange m_range;

};

/* ************************************************************************ */

/**
 * @brief Compares token to token type.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param code Tested token indentifier.
 *
 * @return Has token given token type?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator==(const Code& code, const BasicToken<Code, Value, Position>& rhs) noexcept
{
    return (code == rhs.code);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token type.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param code Tested token indentifier.
 *
 * @return Has token given token type?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator==(const BasicToken<Code, Value, Position>& lhs, const Code& code) noexcept
{
    return (lhs.code == code);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Has token given token value?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator==(const Value& value, const BasicToken<Code, Value, Position>& rhs) noexcept
{
    return (value == rhs.value);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Has token given token value?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator==(const BasicToken<Code, Value, Position>& lhs, const Value& value) noexcept
{
    return (lhs.value == value);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Has token given token value?
 */
template<typename Code, typename Value, typename Position, typename T>
inline
typename std::enable_if<std::is_constructible<Value, T>::value, bool>::type
operator==(const T& value, const BasicToken<Code, Value, Position>& rhs) noexcept
{
    return operator==(Value{value}, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Has token given token value?
 */
template<typename Code, typename Value, typename Position, typename T>
inline
typename std::enable_if<std::is_constructible<Value, T>::value, bool>::type
operator==(const BasicToken<Code, Value, Position>& lhs, const T& value) noexcept
{
    return operator==(lhs, Value{value});
}

/* ************************************************************************ */

/**
 * @brief Compares token to token type.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param code Tested token indentifier.
 *
 * @return Hasn't token given token type?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator!=(const Code& code, const BasicToken<Code, Value, Position>& rhs) noexcept
{
    return !operator==(code, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token type.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param code Tested token indentifier.
 *
 * @return Hasn't token given token type?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator!=(const BasicToken<Code, Value, Position>& lhs, const Code& code) noexcept
{
    return !operator==(lhs, code);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Hasn't token given token value?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator!=(const Value& value, const BasicToken<Code, Value, Position>& rhs) noexcept
{
    return !operator==(value, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Hasn't token given token value?
 */
template<typename Code, typename Value, typename Position>
inline
bool operator!=(const BasicToken<Code, Value, Position>& lhs, const Value& value) noexcept
{
    return !operator==(lhs, value);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Hasn't token given token value?
 */
template<typename Code, typename Value, typename Position, typename T>
inline
typename std::enable_if<std::is_constructible<Value, T>::value, bool>::type
operator!=(const T& value, const BasicToken<Code, Value, Position>& rhs) noexcept
{
    return operator!=(Value{value}, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compares token to token value.
 *
 * @tparam Code A type that allows faster token type identification.
 * @tparam Value A type for storing token value. It can be a sequence of
 * token characters or range of iterators.
 * @tparam Position A type for storing token position start position.
 *
 * @param value Tested value.
 *
 * @return Hasn't token given token value?
 */
template<typename Code, typename Value, typename Position, typename T>
inline
typename std::enable_if<std::is_constructible<Value, T>::value, bool>::type
operator!=(const BasicToken<Code, Value, Position>& lhs, const T& value) noexcept
{
    return !operator==(lhs, Value{value});
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
