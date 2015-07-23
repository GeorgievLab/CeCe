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

// C++
#include <cmath>
#include <limits>
#include <type_traits>

// Simulator
#include "core/Zero.hpp"
#include "core/String.hpp"
#include "core/StaticArray.hpp"
#include "core/IntegerSequence.hpp"
#include "core/InStream.hpp"
#include "core/OutStream.hpp"
#include "core/Exception.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Basic value.
 */
using Value = float;

/* ************************************************************************ */

/// Base units coefficients
static constexpr Value LENGTH_COEFFICIENT = 1e6f;
static constexpr Value TIME_COEFFICIENT = 1e0f;
static constexpr Value MASS_COEFFICIENT = 1e9f;

/* ************************************************************************ */

/// @link https://azraelplanet.wordpress.com/2012/03/30/number-of-arguments-in-__va_args__-gcc/
#define ARG_N(_1, _2, _3, N, ...) N
#define NARG_(...) ARG_N(__VA_ARGS__)
#define NARG(...) NARG_(__VA_ARGS__, 3, 2, 1, 0)

/* ************************************************************************ */

/**
 * @brief Defines base unit.
 *
 * @param name  Unit Name.
 * @param ord   Unit order.
 * @param coeff Value coefficient.
 * @param ...   Characters of unit symbol.
 */
#define DEFINE_BASE_UNIT(name, coeff, ord, ...) \
    struct Base ## name {\
        static constexpr Value coefficient = coeff; \
        static constexpr int order = ord;\
        static constexpr unsigned int symbolLength = NARG(__VA_ARGS__) + 1; \
        static constexpr StaticArray<char, symbolLength> getSymbol() noexcept \
        { \
            return {__VA_ARGS__, '\0'};\
        } \
    }

/* ************************************************************************ */

/**
 * @brief Base SI units.
 */
DEFINE_BASE_UNIT(Length, LENGTH_COEFFICIENT,    0, 'm');
DEFINE_BASE_UNIT(Time,   TIME_COEFFICIENT,      1, 's');
DEFINE_BASE_UNIT(Mass,   MASS_COEFFICIENT,      2, 'g');
DEFINE_BASE_UNIT(ElectricCurrent, 1.f,          3, 'A');
DEFINE_BASE_UNIT(ThermodynamicTemperature, 1.f, 4, 'K');
DEFINE_BASE_UNIT(AmountOfSubstance, 1.f,        5, 'm', 'o', 'l');
DEFINE_BASE_UNIT(LuminousIntensity, 1.f,        6, 'c', 'd');

/* ************************************************************************ */

/**
 * @brief Less structure.
 *
 * @tparam T1 First base unit type.
 * @tparam T2 Second base unit type.
 */
template<typename T1, typename T2>
struct Less
{
    /// If T1 is less than T2
    static constexpr bool value = T1::order < T2::order;
};

/* ************************************************************************ */

/**
 * @brief List of types.
 *
 * @tparam Types A list of types.
 */
template<typename... Types>
struct List
{

    /// Number of types in list.
    static constexpr unsigned int size = sizeof...(Types);

};

/* ************************************************************************ */

/**
 * @brief Struct for calculating total coefficient of given list.
 *
 * @tparam Types A list of unit types.
 */
template<typename... Types>
struct Coefficient;

/* ************************************************************************ */

/**
 * @brief Struct for calculating total coefficient of given list.
 *
 * @tparam Types A list of unit types.
 */
template<typename... Types>
struct Coefficient<List<Types...>>
{

    /**
     * @brief Multiply coefficients.
     *
     * @param arg  The first coefficient.
     * @param args Rest of the coefficients.
     *
     * @return Result coefficient.
     */
    template<typename Arg, typename... Args>
    static constexpr Value multiply(Arg&& arg, Args&&... args) noexcept
    {
        return arg * multiply(args...);
    }


    /**
     * @brief Multiply coefficients.
     *
     * @return 1.
     */
    static constexpr Value multiply() noexcept
    {
        return Value(1);
    }


    /// Calculate types coefficient.
    static constexpr Value value = multiply(Types::coefficient...);

};

/* ************************************************************************ */

/**
 * @brief Concatenate two lists.
 *
 * @tparam T Types.
 */
template<typename... T>
struct Concat;

/* ************************************************************************ */

/**
 * @brief Concatenated specialization for single list.
 *
 * @tparam Types Types of the list.
 */
template<typename... Types>
struct Concat<List<Types...>>
{
    /// Concatenated list.
    using type = List<Types...>;
};

/* ************************************************************************ */

/**
 * @brief Concatenate two lists.
 *
 * @tparam Types1 Types of the first list.
 * @tparam Types2 Types of the second list.
 * @tparam Tail   Remaining types.
 */
template<typename... Types1, typename... Types2, typename... Tail>
struct Concat<List<Types1...>, List<Types2...>, Tail...>
{
    /// Concatenated list.
    using type = typename Concat<List<Types1..., Types2...>, Tail...>::type;
};

/* ************************************************************************ */

/**
 * @brief Remove type from list.
 *
 * @tparam T    Type to remove.
 * @tparam List List of types.
 */
template<typename T, typename List>
struct Remove;

/* ************************************************************************ */

/**
 * @brief Remove type from list.
 *
 * @tparam T Type to remove.
 */
template<typename T>
struct Remove<T, List<>>
{
    // Not found
    static constexpr bool value = false;

    // Remaining types.
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Remove type element from list.
 *
 * @tparam T     Type to remove.
 * @tparam Type  First type.
 * @tparam Types Remaining types in the list.
 */
template<typename T, typename Type, typename... Types>
struct Remove<T, List<Type, Types...>>
{
    /// If types match.
    static constexpr bool match = std::is_same<T, Type>::value;

    // Inner remove.
    using RemoveInnerType = Remove<T, List<Types...>>;

    // If type is found.
    static constexpr bool value = match || RemoveInnerType::value;

    /// List type without the required type.
    using type = typename std::conditional<match,
        List<Types...>,
        typename Concat<List<Type>, typename RemoveInnerType::type>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Helper class to reduce units.
 *
 * @tparam Nom   Nominators.
 * @tparam Denom Denominators.
 */
template<typename Nom, typename Denom>
struct ReduceInner;

/* ************************************************************************ */

/**
 * @brief Helper class to reduce units.
 *
 * @tparam Denominators List types.
 */
template<typename... Denominators>
struct ReduceInner<List<>, List<Denominators...>>
{
    using nominators = List<>;
    using denominators = List<Denominators...>;
};

/* ************************************************************************ */

/**
 * @brief Helper class to simplify units.
 *
 * Type removes shared types in nominator and denominator.
 *
 * @tparam Nom          First nominator.
 * @tparam Nominators   List types.
 * @tparam Denominators List types.
 */
template<typename Nom, typename... Nominators, typename... Denominators>
struct ReduceInner<List<Nom, Nominators...>, List<Denominators...>>
{
    /// Type of removing type.
    using remove_type = Remove<Nom, List<Denominators...>>;

    // Reduce without the first nominator
    using reduce_inner = ReduceInner<List<Nominators...>, typename remove_type::type>;

    /// List of nominators
    using nominators = typename std::conditional<remove_type::value,
        typename reduce_inner::nominators,
        typename Concat<List<Nom>, typename reduce_inner::nominators>::type
    >::type;

    /// List of denominators
    using denominators = typename reduce_inner::denominators;
};

/* ************************************************************************ */

/**
 * @brief Remove all occurences of given type element from list.
 *
 * @tparam T    Type to remove.
 * @tparam List List of types.
 */
template<typename T, typename List>
struct RemoveAll;

/* ************************************************************************ */

/**
 * @brief Remove all occurences of given type element from list.
 *
 * @tparam T Type to remove.
 */
template<typename T>
struct RemoveAll<T, List<>>
{
    // Not found
    static constexpr bool value = false;

    // Number of occurences.
    static constexpr unsigned int count = 0;

    // Remaining types.
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Remove all occurences of given type element from list.
 *
 * @tparam T     Type to remove.
 * @tparam Type  First type.
 * @tparam Types Remaining types in the list.
 */
template<typename T, typename Type, typename... Types>
struct RemoveAll<T, List<Type, Types...>>
{
    /// If types match.
    static constexpr bool match = std::is_same<T, Type>::value;

    // Inner remove.
    using RemoveInnerType = RemoveAll<T, List<Types...>>;

    // If type is found.
    static constexpr bool value = match || RemoveInnerType::value;

    // Number of occurences.
    static constexpr unsigned int count = (match ? 1 : 0) + RemoveInnerType::count;

    /// List type without the required type.
    using type = typename std::conditional<match,
        typename RemoveInnerType::type,
        typename Concat<List<Type>, typename RemoveInnerType::type>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Group of unit symbol.
 *
 * @tparam Type  Base unit type.
 * @tparam Count Number of base unit occurences.
 */
template<typename Type, unsigned int Count>
struct SymbolGroup
{
    static_assert(Count > 1, "Count must be greater than 1");
    static_assert(Count < 10, "Count must be less than 10");


    /// Length of the base suffix.
    static constexpr unsigned int length = Type::symbolLength + 1;


    /**
     * @brief Create symbol character array.
     *
     * @tparam Indices Indices from symbol array to use.
     *
     * @param symbol  Type symbol.
     * @param indices Sequence of indices.
     *
     * @return Result symbol.
     */
    template<int... Indices>
    static constexpr StaticArray<char, length> createSymbol(
        const StaticArray<char, length - 1>& symbol,
        IntegerSequence<Indices...> indices
    ) noexcept
    {
        return {std::get<Indices>(symbol)..., '0' + Count, '\0'};
    }


    /**
     * @brief Returns symbol character array.
     *
     * @return
     */
    static constexpr StaticArray<char, length> get() noexcept
    {
        return createSymbol(Type::getSymbol(), MakeIntegerSequence<0, length - 2>{});
    }
};

/* ************************************************************************ */

/**
 * @brief Group of unit symbol specialization for single occurence.
 *
 * @tparam Type  Base unit type.
 */
template<typename Type>
struct SymbolGroup<Type, 1>
{
    /// Length of the base suffix.
    static constexpr unsigned int length = Type::symbolLength;


    /**
     * @brief Returns symbol character array.
     *
     * @return
     */
    static constexpr StaticArray<char, length> get() noexcept
    {
        return Type::getSymbol();
    }
};

/* ************************************************************************ */

/**
 * @brief Create SymbolGroup for list of types.
 *
 * @tparam Types A list of types.
 */
template<typename... Types>
struct SymbolGrouper;

/* ************************************************************************ */

/**
 * @brief Create SymbolGroup for list of types.
 *
 * @tparam Types A list of types.
 */
template<>
struct SymbolGrouper<List<>>
{
    /// List of suffix groups
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Create SymbolGroup for list of types.
 *
 * @tparam Type  The first type.
 * @tparam Types A list of types.
 */
template<typename Type, typename... Types>
struct SymbolGrouper<List<Type, Types...>>
{
    using RemoveType = RemoveAll<Type, List<Type, Types...>>;

    /// List of suffix groups.
    using type = typename Concat<
        List<SymbolGroup<Type, RemoveType::count>>,
        typename SymbolGrouper<typename RemoveType::type>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Symbol length.
 *
 * @tparam Types List of types.
 */
template<typename... Types>
struct SymbolLength;

/* ************************************************************************ */

/**
 * @brief Symbol length.
 *
 * @tparam Types List of types.
 */
template<typename... Types>
struct SymbolLength<List<Types...>>
{

    /**
     * @brief Add lengths.
     *
     * @param arg  The first length.
     * @param args Rest of the lengths.
     *
     * @return Total length.
     */
    template<typename Arg, typename... Args>
    static constexpr Value add(Arg&& arg, Args&&... args) noexcept
    {
        return arg + add(args...);
    }


    /**
     * @brief Add length.
     *
     * @return 0.
     */
    static constexpr Value add() noexcept
    {
        return Value(0);
    }


    /// Symbol length with '\0' and without '\0' of each type.
    static constexpr unsigned int value = add(Types::length...) - sizeof...(Types) + 1;

};

/* ************************************************************************ */

/**
 * @brief Unit type symbol.
 *
 * @tparam Types List of base types.
 */
template<typename... Types>
struct Symbol;

/* ************************************************************************ */

/**
 * @brief Unit type symbol.
 *
 * @tparam Types List of base types.
 */
template<typename... Types>
struct Symbol<List<Types...>>
{

    /// Symbol length.
    static constexpr unsigned int length = SymbolLength<List<Types...>>::value;


    /**
     * @brief Concatenate two symbols.
     *
     * @tparam N1 Length of the first symbol.
     * @tparam N2 Length of the second symbol.
     * @tparam Indices1 Indices for the first symbol.
     * @tparam Indices2 Indices for the second symbol.
     *
     * @param symbol1 The first symbol.
     * @param seq1 Indices for the first symbol.
     * @param symbol2 The second symbol.
     * @param seq2 Indices for the second symbol.
     *
     * @return Result symbol.
     */
    template<unsigned long N1, unsigned long N2, int... Indices1, int... Indices2>
    static constexpr StaticArray<char, N1 + N2 - 1> concat(
        const StaticArray<char, N1>& symbol1,
        IntegerSequence<Indices1...> seq1,
        const StaticArray<char, N2>& symbol2,
        IntegerSequence<Indices2...> seq2
    ) noexcept
    {
        static_assert(N1 + N2 - 1 == sizeof...(Indices1) + sizeof...(Indices2) + 1, "Sizes doesn't match");

        return {std::get<Indices1>(symbol1)..., std::get<Indices2>(symbol2)..., '\0'};
    }


    /**
     * @brief Create empty symbol.
     *
     * @return Empty array (only '\0').
     */
    static constexpr StaticArray<char, 1> createSymbol() noexcept
    {
        return {'\0'};
    }


    /**
     * @brief Create symbol from single symbol.
     *
     * @tparam N1 Length of the symbol.
     *
     * @param symbol Source symbol.
     */
    template<unsigned long N1>
    static constexpr StaticArray<char, N1> createSymbol(StaticArray<char, N1> symbol) noexcept
    {
        return symbol;
    }


    /**
     * @brief Create symbol from two symbols.
     *
     * @tparam N1 Length of the first symbol.
     * @tparam N2 Length of the second symbol.
     *
     * @param symbol1 The first symbol.
     * @param symbol2 The second symbol.
     *
     * @return Result symbol.
     */
    template<unsigned long N1, unsigned long N2>
    static constexpr StaticArray<char, N1 + N2 - 1> createSymbol(
        const StaticArray<char, N1>& symbol1,
        const StaticArray<char, N2>& symbol2
    ) noexcept
    {
        return concat(
            symbol1, MakeIntegerSequence<0, N1 - 1>{},
            symbol2, MakeIntegerSequence<0, N2 - 1>{}
        );
    }


    /**
     * @brief Create symbol from three symbols.
     *
     * @tparam N1 Length of the first symbol.
     * @tparam N2 Length of the second symbol.
     * @tparam N3 Length of the third symbol.
     *
     * @param symbol1 The first symbol.
     * @param symbol2 The second symbol.
     * @param symbol3 The third symbol.
     *
     * @return Result symbol.
     */
    template<unsigned long N1, unsigned long N2, unsigned long N3>
    static constexpr StaticArray<char, N1 + N2 + N3 - 2> createSymbol(
        const StaticArray<char, N1>& symbol1,
        const StaticArray<char, N2>& symbol2,
        const StaticArray<char, N3>& symbol3) noexcept
    {
        return createSymbol(symbol1, createSymbol(symbol2, symbol3));
    }


    /**
     * @brief Returns suffix array string.
     */
#if 0
    // TODO: support for more symbols
    template<unsigned long N1, typename... Arrays>
    static constexpr auto createSymbol(const StaticArray<char, N1>& a1, Arrays&&... arrays) noexcept
        -> decltype(createSymbol(a1, createSymbol(std::forward<Arrays>(arrays)...)))
    {
        static_assert(sizeof...(Arrays) > 2, "Damn");

        return createSymbol(a1, createSymbol(std::forward<Arrays>(arrays)...));
    }
#endif


    /**
     * @brief Returns symbol character array.
     *
     * @return
     */
    static constexpr StaticArray<char, length> get() noexcept
    {
        return createSymbol(Types::get()...);
    }
};

/* ************************************************************************ */

/**
 * @brief Symbol for unit type.
 *
 * @tparam Nominators   List of nominators.
 * @tparam Denominators List of denominators.
 */
template<typename... Nominators, typename... Denominators>
struct Symbol<List<Nominators...>, List<Denominators...>>
{
    /// Nominators symbol.
    using SymbolNominators = Symbol<typename SymbolGrouper<List<Nominators...>>::type>;

    /// Denominators symbol.
    using SymbolDenominators = Symbol<typename SymbolGrouper<List<Denominators...>>::type>;


    /// Length of nominator symbol.
    static constexpr unsigned int nomLength = SymbolNominators::length;

    /// Length of denominator symbol.
    static constexpr unsigned int denomLength = SymbolDenominators::length;

    /// Total symbol length.
    static constexpr unsigned int length = nomLength + (denomLength == 1 ? 0 : denomLength);


    /**
     * @brief Concatenate nominator and denominator symbols.
     *
     * @tparam IndicesNom   Indices for nominator symbol.
     * @tparam IndicesDenom Indices for denominator symbol.
     *
     * @param nominatorSymbol   Nominator symbol.
     * @param denominatorSymbol Denominator symbol.
     *
     * @return Result symbol.
     */
    template<int... IndicesNom, int... IndicesDenom>
    static constexpr StaticArray<char, length> concat(
        const StaticArray<char, nomLength>& nominatorSymbol,
        IntegerSequence<IndicesNom...>,
        const StaticArray<char, denomLength>& denominatorSymbol,
        IntegerSequence<IndicesDenom...>
    ) noexcept
    {
        return {
            std::get<IndicesNom>(nominatorSymbol)...,
            '/',
            std::get<IndicesDenom>(denominatorSymbol)...,
            '\0'
        };
    }


    /**
     * @brief Create symbol without denominator symbol.
     *
     * @tparam IndicesNom Indices for nominator symbol.
     *
     * @param nominatorSymbol   Nominator symbol.
     *
     * @return Result symbol.
     */
    template<int... IndicesNom>
    static constexpr StaticArray<char, nomLength> concat(
        const StaticArray<char, nomLength>& nominatorSymbol,
        IntegerSequence<IndicesNom...>,
        const StaticArray<char, 1>&,
        IntegerSequence<>
    ) noexcept
    {
        return {std::get<IndicesNom>(nominatorSymbol)..., '\0'};
    }


    /**
     * @brief Returns symbol character array.
     *
     * @return
     */
    static constexpr StaticArray<char, length> get() noexcept
    {
        return concat(
            SymbolNominators::get(),
            MakeIntegerSequence<0, nomLength - 1>{},
            SymbolDenominators::get(),
            MakeIntegerSequence<0, denomLength - 1>{}
        );
    }
};

/* ************************************************************************ */

/**
 * @brief SI Unit.
 *
 * @tparam Nom   List type.
 * @tparam Denom List type.
 */
template<typename Nom, typename Denom>
class Unit
{

// Public Types
public:


    /// Value type.
    using value_type = Value;

    /// List type.
    using nominator = Nom;

    /// List type.
    using denominator = Denom;

    /// Unit symbol type.
    using symbol = Symbol<Nom, Denom>;


// Public Constants
public:


    /// Total unit coefficient.
    static constexpr Value coefficient = Coefficient<Nom>::value / Coefficient<Denom>::value;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Unit() = default;


    /**
     * @brief Constructor.
     *
     * @param value Init value.
     */
    explicit constexpr Unit(value_type value) noexcept
        : m_value(value)
    {
        // Nothing to do
    }


    /**
     * @brief Zero constructor.
     */
    constexpr Unit(Zero_t) noexcept
        : m_value(0)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief If value is set operator.
     *
     * @return
     */
    explicit operator bool() const noexcept
    {
        return m_value != 0;
    }


    /**
     * @brief Cast to value type.
     *
     * @return
     */
    explicit operator Value() const noexcept
    {
        return m_value;
    }


    /**
     * @brief Unary plus operator.
     *
     * @return New value.
     */
    Unit operator+() const noexcept
    {
        return Unit(m_value);
    }


    /**
     * @brief Unary minus operator.
     *
     * @return New value.
     */
    Unit operator-() const noexcept
    {
        return Unit(-m_value);
    }


    /**
     * @brief Addition operator.
     *
     * @param rhs
     *
     * @return *this.
     */
    Unit& operator+=(Unit rhs) noexcept
    {
        m_value += rhs.m_value;
        return *this;
    }


    /**
     * @brief Substraction operator.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    Unit& operator-=(Unit rhs) noexcept
    {
        m_value -= rhs.m_value;
        return *this;
    }


    /**
     * @brief Multiplication operator.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    Unit& operator*=(value_type rhs) noexcept
    {
        m_value *= rhs;
        return *this;
    }


    /**
     * @brief Division operator.
     *
     * @param rhs Right operand.
     *
     * @return *this.
     */
    Unit& operator/=(value_type rhs) noexcept
    {
        m_value /= rhs;
        return *this;
    }


// Public Accessors
public:


    /**
     * @brief Returns current value.
     *
     * @return
     */
    constexpr value_type value() const noexcept
    {
        return m_value;
    }


// Private Data Members
private:

    /// Stored value.
    value_type m_value;

};

/* ************************************************************************ */

/**
 * @brief Reduce empty lists.
 */
template<typename Nominators, typename Denominators>
struct ReduceEmpty
{
    /// Result unit type.
    using type = Unit<Nominators, Denominators>;
};

/* ************************************************************************ */

/**
 * @brief Reduce empty lists.
 */
template<>
struct ReduceEmpty<List<>, List<>>
{
    /// Result unit type.
    using type = Value;
};

/* ************************************************************************ */

/**
 * @brief Value filter.
 *
 * @tparam T     Type to filter.
 * @tparam neg   Negate less.
 * @tparam Types Types.
 */
template<typename T, bool neg, typename Types>
struct Filter;

/* ************************************************************************ */

/**
 * @brief Value filter.
 *
 * @tparam T     Type to filter.
 * @tparam neg   Negate less.
 * @tparam Type  First type.
 * @tparam Types Types.
 */
template<typename T, bool neg, typename Type, typename... Types>
struct Filter<T, neg, List<Type, Types...>>
{
    // List without the first type.
    using tail = typename Filter<T, neg, List<Types...>>::type;

    using type = typename std::conditional<
        neg ^ Less<T, Type>::value,
        typename Concat<List<Type>, tail>::type,
        tail
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Value filter.
 *
 * @tparam T     Type to filter.
 * @tparam neg   Negate less.
 */
template<typename T, bool neg>
struct Filter<T, neg, List<>>
{
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 *
 * @tparam Types Types.
 */
template<typename Types>
struct Sort;

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 *
 * @tparam Type  First type.
 * @tparam Types A list of types.
 */
template<typename Type, typename... Types>
struct Sort<List<Type, Types...>>
{
    using front = typename Filter<Type, true, List<Types...>>::type;
    using tail = typename Filter<Type, false, List<Types...>>::type;

    // Sorted list.
    using type = typename Concat<front, List<Type>, tail>::type;
};

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 *
 * @tparam Type Type.
 */
template<typename Type>
struct Sort<List<Type>>
{
    using type = List<Type>;
};

/* ************************************************************************ */

/**
 * @brief List sorting structure.
 */
template<>
struct Sort<List<>>
{
    using type = List<>;
};

/* ************************************************************************ */

/**
 * @brief Helper class to reduce units.
 *
 * @tparam Nom   Nominators.
 * @tparam Denom Denominators.
 */
template<typename Nom, typename Denom>
struct Reduce;

/* ************************************************************************ */

/**
 * @brief Helper class to simplify units.
 *
 * Type removes shared types in nominator and denominator.
 *
 * @tparam Nom          First nominator.
 * @tparam Nominators   List types.
 * @tparam Denominators List types.
 */
template<typename... Nominators, typename... Denominators>
struct Reduce<List<Nominators...>, List<Denominators...>>
{
    // Inner reduce
    using inner = ReduceInner<List<Nominators...>, List<Denominators...>>;

    /// Result unit type.
    using type = typename ReduceEmpty<
        typename Sort<typename inner::nominators>::type,
        typename Sort<typename inner::denominators>::type
    >::type;
};

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator==(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return std::abs(lhs.value() - rhs.value()) < std::numeric_limits<Value>::epsilon();
    //return lhs.value() == rhs.value();
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator==(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t rhs
) noexcept
{
    return lhs == Unit<List<Nominators...>, List<Denominators...>>(Zero);
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator==(
    Zero_t lhs, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return Unit<List<Nominators...>, List<Denominators...>>(Zero) == rhs;
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator!=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator!=(
    Unit<List<Nominators...>, List<Denominators...>> lhs, Zero_t rhs
) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Compare operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator!=(
    Zero_t lhs, Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator==(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Less operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return lhs.value() < rhs.value();
}

/* ************************************************************************ */

/**
 * @brief Less equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator<=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator>(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Greater operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return operator<(rhs, lhs);
}

/* ************************************************************************ */

/**
 * @brief Greater equals operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr bool operator>=(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return !operator<(lhs, rhs);
}

/* ************************************************************************ */

/**
 * @brief Addition operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator+(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return lhs += rhs;
}

/* ************************************************************************ */

/**
 * @brief Substraction operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator-(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return lhs -= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator*(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type rhs) noexcept
{
    return lhs *= rhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator*(
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return rhs *= lhs;
}

/* ************************************************************************ */

/**
 * @brief Multiplication operator.
 *
 * @tparam Nominators1   A list of nominators.
 * @tparam Denominators1 A list of denominators.
 * @tparam Nominators2   A list of nominators.
 * @tparam Denominators2 A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators1, typename... Denominators1, typename... Nominators2, typename... Denominators2>
inline constexpr
typename Reduce<List<Nominators1..., Nominators2...>, List<Denominators1..., Denominators2...>>::type operator*(
    Unit<List<Nominators1...>, List<Denominators1...>> lhs,
    Unit<List<Nominators2...>, List<Denominators2...>> rhs
) noexcept
{
    return typename Reduce<List<Nominators1..., Nominators2...>, List<Denominators1..., Denominators2...>>::type{
        lhs.value() * rhs.value()
    };
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator/(
    Unit<List<Nominators...>, List<Denominators...>> lhs,
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type rhs
) noexcept
{
    return lhs /= rhs;
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam Nominators   A list of nominators.
 * @tparam Denominators A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators, typename... Denominators>
inline constexpr
Unit<List<Nominators...>, List<Denominators...>> operator/(
    typename Unit<List<Nominators...>, List<Denominators...>>::value_type lhs,
    Unit<List<Nominators...>, List<Denominators...>> rhs
) noexcept
{
    return rhs /= lhs;
}

/* ************************************************************************ */

/**
 * @brief Dividing operator.
 *
 * @tparam Nominators1   A list of nominators.
 * @tparam Denominators1 A list of denominators.
 * @tparam Nominators2   A list of nominators.
 * @tparam Denominators2 A list of denominators.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 *
 * @return Result value.
 */
template<typename... Nominators1, typename... Denominators1, typename... Nominators2, typename... Denominators2>
inline constexpr
typename Reduce<List<Nominators1..., Denominators2...>, List<Denominators1..., Nominators2...>>::type operator/(
    Unit<List<Nominators1...>, List<Denominators1...>> lhs,
    Unit<List<Nominators2...>, List<Denominators2...>> rhs
) noexcept
{
    return typename Reduce<List<Nominators1..., Denominators2...>, List<Denominators1..., Nominators2...>>::type{
        lhs.value() / rhs.value()
    };
}

/* ************************************************************************ */

/**
 * @brief Delta type - used for change nominator per denominator.
 *
 * @tparam Nom
 * @tparam Denom
 */
template<typename Nom, typename Denom>
struct Delta
{
    using type = decltype(Nom{} / Denom{});
};

/* ************************************************************************ */

/**
 * @brief Class for representing distance (meters).
 */
using Length = Unit<List<BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing mass (kilograms).
 */
using Mass = Unit<List<BaseMass>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing time (seconds).
 */
using Time = Unit<List<BaseTime>, List<>>;
using Duration = Time;

/* ************************************************************************ */

/**
 * @brief Class for representing area.
 */
using Area = Unit<List<BaseLength, BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Volume = Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;

/* ************************************************************************ */

/**
 * @brief Class for representing velocity (micrometers per second).
 */
using Velocity = Unit<List<BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing acceleration (micrometers per second^2).
 */
using Acceleration = Unit<List<BaseLength>, List<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing force (Newton).
 */
using Force = Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing density.
 */
using Density = Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;

/* ************************************************************************ */

/**
 * @brief Class for representing dynamic viscosity.
 */
using DynamicViscosity = Unit<List<BaseMass>, List<BaseLength, BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing kinematic viscosity.
 */
using KinematicViscosity = Unit<List<BaseLength, BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

/**
 * @brief Class for representing volume.
 */
using Angle = Value;

/* ************************************************************************ */

/**
 * @brief Class for representing probability.
 */
using Probability = Value;

/* ************************************************************************ */

#if 0
extern template class Unit<List<BaseLength>, List<>>;
extern template class Unit<List<BaseMass>, List<>>;
extern template class Unit<List<BaseTime>, List<>>;
extern template class Unit<List<BaseLength, BaseLength>, List<>>;
extern template class Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
extern template class Unit<List<BaseLength>, List<BaseTime>>;
extern template class Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
extern template class Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;
extern template class Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;
extern template class Unit<List<BaseMass>, List<BaseLength, BaseTime>>;
extern template class Unit<List<BaseLength, BaseLength>, List<BaseTime>>;
#endif

/* ************************************************************************ */

/**
 * @brief Convert degrees to radians.
 *
 * @param value
 *
 * @return
 */
inline constexpr Value deg2rad(Value value) noexcept
{
    return value * 0.01745329252f;
}

/* ************************************************************************ */

/**
 * @brief Convert radians to degrees.
 *
 * @param value
 *
 * @return
 */
inline constexpr Value rad2deg(Value value) noexcept
{
    return value * 57.2957795f;
}

/* ************************************************************************ */

/**
 * @brief Meter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length m(Value value) noexcept
{
    // 1m = 1'000mm
    return Length(LENGTH_COEFFICIENT * value);
}

/* ************************************************************************ */

/**
 * @brief Millimeter value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length mm(Value value) noexcept
{
    // 1mm = 1/1'000m
    return m(value * 1e-3f);
}

/* ************************************************************************ */

/**
 * @brief Micrometer value.
 *
 * @param value Value.
 *
 * @return Length value.
 */
inline constexpr Length um(Value value) noexcept
{
    // 1um = 1/1'000'000m
    return m(value * 1e-6f);
}

/* ************************************************************************ */

/**
 * @brief Kilograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass kg(Value value) noexcept
{
    return Mass(MASS_COEFFICIENT * value);
}

/* ************************************************************************ */

/**
 * @brief Grams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass g(Value value) noexcept
{
    // 1g = 1/1'000 kg
    return kg(value * 1e-3f);
}

/* ************************************************************************ */

/**
 * @brief Milligrams value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass mg(Value value) noexcept
{
    // 1mg = 1/1'000 g
    return g(value * 1e-3f);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass ug(Value value) noexcept
{
    // 1ug = 1/1'000 mg
    return mg(value * 1e-3f);
}

/* ************************************************************************ */

/**
 * @brief Nanograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass ng(Value value) noexcept
{
    // 1ng = 1/1'000 ug
    return ug(value * 1e-3f);
}

/* ************************************************************************ */

/**
 * @brief Micrograms value.
 *
 * @param value Value.
 *
 * @return Mass value.
 */
inline constexpr Mass pg(Value value) noexcept
{
    // 1pg = 1/1'000 ng
    return ng(value * 1e-3f);
}

/* ************************************************************************ */

/**
 * @brief Seconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time s(Value value) noexcept
{
    // 1s
    return Time(TIME_COEFFICIENT * value);
}

/* ************************************************************************ */

/**
 * @brief Milliseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time ms(Value value) noexcept
{
    // 1s = 1'000ms
    return s(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Microseconds value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time us(Value value) noexcept
{
    // 1ms = 1'000us
    return ms(value / 1000.f);
}

/* ************************************************************************ */

/**
 * @brief Minutes value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time min(Value value) noexcept
{
    // 60s = 1min
    return s(60 * value);
}

/* ************************************************************************ */

/**
 * @brief Hours value.
 *
 * @param value Value.
 *
 * @return Time value.
 */
inline constexpr Time h(Value value) noexcept
{
    // 60min = 1h
    return min(60 * value);
}

/* ************************************************************************ */

/**
 * @brief Meters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area m2(Value value) noexcept
{
    return m(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area mm2(Value value) noexcept
{
    return mm(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^2 value.
 *
 * @param value Value.
 *
 * @return Area value.
 */
inline constexpr Area um2(Value value) noexcept
{
    return um(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume m3(Value value) noexcept
{
    return m2(value) * m(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume mm3(Value value) noexcept
{
    return mm2(value) * mm(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers^3 value.
 *
 * @param value Value.
 *
 * @return Volume value.
 */
inline constexpr Volume um3(Value value) noexcept
{
    return um2(value) * um(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline constexpr Velocity m_s(Value value) noexcept
{
    return m(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline constexpr Velocity mm_s(Value value) noexcept
{
    return mm(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second value.
 *
 * @param value Value.
 *
 * @return Velocity value.
 */
inline constexpr Velocity um_s(Value value) noexcept
{
    return um(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Meters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline constexpr Acceleration m_s2(Value value) noexcept
{
    return m_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeters per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline constexpr Acceleration mm_s2(Value value) noexcept
{
    return mm_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometers per second^2 value.
 *
 * @param value Value.
 *
 * @return Acceleration value.
 */
inline constexpr Acceleration um_s2(Value value) noexcept
{
    return um_s(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Kilogram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force kgm_s2(Value value) noexcept
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Gram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force gm_s2(Value value) noexcept
{
    return g(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Milligram meter per second^2 value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force mgm_s2(Value value) noexcept
{
    return kg(value) * m_s2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force N(Value value) noexcept
{
    return kgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Millinewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force mN(Value value) noexcept
{
    return gm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Micronewton value.
 *
 * @param value Value.
 *
 * @return Force value.
 */
inline constexpr Force uN(Value value) noexcept
{
    return mgm_s2(value);
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity kg_m_s(Value value) noexcept
{
    return kg(value) / (m(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Kilogram per meter and second value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity g_m_s(Value value) noexcept
{
    return g(value) / (m(1) * s(1));
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity Ns_m2(Value value) noexcept
{
    return N(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity Pas(Value value) noexcept
{
    return N(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Newton second per meter square value.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr DynamicViscosity mPas(Value value) noexcept
{
    return mN(value) * s(1) / m2(1);
}

/* ************************************************************************ */

/**
 * @brief Meter square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr KinematicViscosity m2_s(Value value) noexcept
{
    return m2(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Millimeter square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr KinematicViscosity mm2_s(Value value) noexcept
{
    return mm2(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Micrometer square per second.
 *
 * @param value Value.
 *
 * @return Viscosity value.
 */
inline constexpr KinematicViscosity um2_s(Value value) noexcept
{
    return um2(value) / s(1);
}

/* ************************************************************************ */

/**
 * @brief Radian constructor.
 *
 * @param value Value in radians.
 *
 * @return Angle.
 */
inline constexpr Angle rad(Value value) noexcept
{
    return Angle(value);
}

/* ************************************************************************ */

/**
 * @brief Degree constructor.
 *
 * @param value Value in degrees.
 *
 * @return Angle.
 */
inline constexpr Angle deg(Value value) noexcept
{
    return rad(deg2rad(value));
}

/* ************************************************************************ */

/**
 * @brief Probability constructor.
 *
 * @param value Value in precent.
 *
 * @return
 */
inline constexpr Probability precent(Value value) noexcept
{
    return value / 100.f;
}

/* ************************************************************************ */

/**
 * @brief Input stream operator.
 *
 * @param is   Input stream.
 * @param unit Result value.
 *
 * @return is.
 */
template<typename... Nominators, typename... Denominators>
InStream& operator>>(InStream& is, Unit<List<Nominators...>, List<Denominators...>>& unit)
{
    using Type = Unit<List<Nominators...>, List<Denominators...>>;

    Value val;
    String symbol;
    is >> val >> symbol;

    // Symbol is given
    if (!symbol.empty())
    {
        // Type symbol
        static constexpr auto typeSymbol = Type::symbol::get();

        // Find real symbol
        auto pos = symbol.find(typeSymbol.data());
        if (pos == String::npos)
        {
            throw InvalidArgumentException(
                "Invalid units symbol. "
                "Expected '(m|u|n)" + String(typeSymbol.data()) + "' "
                "given '" + symbol + "'"
            );
        }

        // Modify base value
        val *= Type::coefficient;

        // Get unit prefix
        if (pos == 1)
        {
            // Unit prefix
            switch (symbol.front())
            {
            default:
                throw InvalidArgumentException("Unknown/unsupported unit symbol prefix: " + String(1, symbol.front()));

            case 'n':
                val *= 1e-3;
            case 'u':
                val *= 1e-3;
            case 'm':
                val *= 1e-3;
            }
        }
        else if (pos != 0)
        {
            throw InvalidArgumentException("Unit symbol prefix is longer than expected");
        }
    }

    // Set unit
    unit = Unit<List<Nominators...>, List<Denominators...>>{val};

    return is;
}

/* ************************************************************************ */

/**
 * @brief Output stream operator.
 *
 * @param os   Output stream.
 * @param unit Input value.
 *
 * @return os.
 */
template<typename... Nominators, typename... Denominators>
OutStream& operator<<(OutStream& os, const Unit<List<Nominators...>, List<Denominators...>>& unit) noexcept
{
    os << unit.value();

    // TODO: write suffix

    return os;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}

/* ************************************************************************ */
