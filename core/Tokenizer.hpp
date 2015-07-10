/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/Log.hpp"
#include "core/Range.hpp"
#include "core/Exception.hpp"
#include "core/StringView.hpp"

/* ************************************************************************ */

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
    constexpr BasicToken() noexcept
        : BasicToken{INVALID}
    {
        // Nothing to do
    }


    /**
     * @brief Creates a token for given token type.
     *
     * @param code Token code.
     */
    explicit constexpr BasicToken(CodeType code) noexcept
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
    constexpr BasicToken(CodeType code, ValueType value) noexcept
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
    constexpr BasicToken(CodeType code, ValueType value, PositionType position) noexcept
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
    constexpr operator bool() const noexcept
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
    constexpr bool isValid() const noexcept
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
    template<typename, typename, typename>
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
        assert(m_tokenizer != nullptr);

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
 */
template<
    typename Derived,
    typename Token,
    typename InputIterator
>
class BasicTokenizer
{
    template<typename>
    friend class BasicTokenizerIterator;


// Public Types
public:


    /// The token type.
    using TokenType = Token;

    /// Output iterator type.
    using IteratorType = BasicTokenizerIterator<Derived>;

    /// Input value type.
    using ValueType = typename std::iterator_traits<InputIterator>::value_type;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param range Source range.
     */
    explicit BasicTokenizer(IteratorRange<InputIterator> range) noexcept
        : m_range{std::move(range)}
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param source Source object.
     */
    template<typename Source>
    explicit BasicTokenizer(Source&& source) noexcept
        : m_range(makeRange(std::forward<Source>(source)))
    {
        // Nothing to do
    }


// Public Accessors
public:


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


// Public Operations
public:


    /**
     * @brief Read next value from input range.
     */
    void next() noexcept
    {
        m_range.advanceBegin();
    }


// Protected Operations
protected:


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
            "Function Derived::tokenize required");

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
    IteratorRange<InputIterator> m_range;

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

/**
 * @brief Expression token codes.
 */
enum class ExpressionTokenCode
{
    Invalid,
    Identifier,
    Number,
    Operator
};

/* ************************************************************************ */

/**
 * @brief Expression tokenizer.
 *
 * @tparam InputIterator Input iterator type.
 */
template<typename InputIterator>
class ExpressionTokenizer
    : public BasicTokenizer<
        ExpressionTokenizer<InputIterator>,
        BasicToken<ExpressionTokenCode, String>,
        InputIterator
    >
{

// Public Types
public:


    // Parent type
    using ParentType = BasicTokenizer<
        ExpressionTokenizer<InputIterator>,
        BasicToken<ExpressionTokenCode, String>,
        InputIterator
    >;

    /// Token type
    using TokenType = typename ParentType::TokenType;


// Public Ctors & Dtors
public:


    using ParentType::ParentType;


// Public Operation
public:


    using ParentType::value;
    using ParentType::next;
    using ParentType::is;
    using ParentType::match;


    /**
     * @brief Tokenize number.
     *
     * @return
     */
    TokenType tokenizeNumber()
    {
        TokenType token{ExpressionTokenCode::Number};

        while (isDigit() || is('.'))
        {
            token.value.push_back(value());
            next();
        }

        // Suffix
        if (is('f'))
        {
            token.value.push_back(value());
            next();
        }

        return token;
    }


    /**
     * @brief Tokenize source.
     *
     * @return
     */
    TokenType tokenize()
    {
        // Skip whitespaces
        while (value() <= 0x20)
            next();

        if (isDigit())
            return tokenizeNumber();

        switch (value())
        {
        case '+':
        case '-':
        case '*':
        case '/':
        {
            auto val = value();
            next();
            return TokenType{ExpressionTokenCode::Operator, String(1, val)};
        }
        }

        return TokenType{};
    }


// Protected Operation
protected:


    /**
     * @brief Check if current value is digit.
     *
     * @return
     */
    bool isDigit() const noexcept
    {
        return value() >= '0' && value() <= '9';
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
