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

// C++
#include <cstddef>
#include <utility>

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/IntegerSequence.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Group of unit symbol.
 *
 * @tparam Type  Base unit type.
 * @tparam Count Number of base unit occurences.
 */
template<typename Type, std::size_t Count>
struct SymbolGroup
{
    static_assert(Count > 1, "Count must be greater than 1");
    static_assert(Count < 10, "Count must be less than 10");


    /// Length of the base suffix.
    static constexpr std::size_t length = Type::symbolLength + 1;


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
    template<std::size_t... Indices>
    static constexpr StaticArray<char, length> createSymbol(
        const StaticArray<char, length - 1>& symbol,
        IntegerSequence<std::size_t, Indices...> indices
    ) noexcept
    {
        return {{symbol[Indices]..., '0' + Count, '\0'}};
    }


    /**
     * @brief Returns symbol character array.
     *
     * @return
     */
    static constexpr StaticArray<char, length> get() noexcept
    {
        return createSymbol(Type::getSymbol(), MakeIntegerSequence<std::size_t, 0, length - 2>{});
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
    static constexpr std::size_t length = Type::symbolLength;


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
    static constexpr std::size_t value = add(Types::length...) - sizeof...(Types) + 1;

};

/* ************************************************************************ */

/**
 * @brief Unit type symbol.
 *
 * @tparam Types List of base types.
 */
template<typename... Types>
struct SymbolBase;

/* ************************************************************************ */

/**
 * @brief Unit type symbol.
 *
 * @tparam Types List of base types.
 */
template<typename... Types>
struct SymbolBase<List<Types...>>
{

    /// Symbol length.
    static constexpr std::size_t length = SymbolLength<List<Types...>>::value;


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
    template<std::size_t N1, std::size_t N2, std::size_t... Indices1, std::size_t... Indices2>
    static constexpr StaticArray<char, N1 + N2 - 1> concat(
        const StaticArray<char, N1>& symbol1,
        IntegerSequence<std::size_t, Indices1...> seq1,
        const StaticArray<char, N2>& symbol2,
        IntegerSequence<std::size_t, Indices2...> seq2
    ) noexcept
    {
        static_assert(N1 + N2 - 1 == sizeof...(Indices1) + sizeof...(Indices2) + 1, "Sizes doesn't match");

        return {{symbol1[Indices1]..., symbol2[Indices2]..., '\0'}};
    }


    /**
     * @brief Create empty symbol.
     *
     * @return Empty array (only '\0').
     */
    static constexpr StaticArray<char, 1> createSymbol() noexcept
    {
        return {{'\0'}};
    }


    /**
     * @brief Create symbol from single symbol.
     *
     * @tparam N1 Length of the symbol.
     *
     * @param symbol Source symbol.
     */
    template<std::size_t N1>
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
    template<std::size_t N1, std::size_t N2>
    static constexpr StaticArray<char, N1 + N2 - 1> createSymbol(
        const StaticArray<char, N1>& symbol1,
        const StaticArray<char, N2>& symbol2
    ) noexcept
    {
        return concat(
            symbol1, MakeIntegerSequence<std::size_t, 0, N1 - 1>{},
            symbol2, MakeIntegerSequence<std::size_t, 0, N2 - 1>{}
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
    template<std::size_t N1, std::size_t N2, std::size_t N3>
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
    template<std::size_t N1, typename... Arrays>
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
struct SymbolBase<List<Nominators...>, List<Denominators...>>
{
    /// Nominators symbol.
    using SymbolNominators = SymbolBase<typename SymbolGrouper<List<Nominators...>>::type>;

    /// Denominators symbol.
    using SymbolDenominators = SymbolBase<typename SymbolGrouper<List<Denominators...>>::type>;


    /// Length of nominator symbol.
    static constexpr std::size_t nomLength = SymbolNominators::length;

    /// Length of denominator symbol.
    static constexpr std::size_t denomLength = SymbolDenominators::length;

    /// Total symbol length.
    static constexpr std::size_t length = nomLength + (denomLength == 1 ? 0 : denomLength);


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
    template<std::size_t... IndicesNom, std::size_t... IndicesDenom>
    static constexpr StaticArray<char, length> concat(
        const StaticArray<char, nomLength>& nominatorSymbol,
        IntegerSequence<std::size_t, IndicesNom...>,
        const StaticArray<char, denomLength>& denominatorSymbol,
        IntegerSequence<std::size_t, IndicesDenom...>
    ) noexcept
    {
        return {{
            nominatorSymbol[IndicesNom]...,
            '/',
            denominatorSymbol[IndicesDenom]...,
            '\0'
        }};
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
    template<std::size_t... IndicesNom>
    static constexpr StaticArray<char, nomLength> concat(
        const StaticArray<char, nomLength>& nominatorSymbol,
        IntegerSequence<std::size_t, IndicesNom...>,
        const StaticArray<char, 1>&,
        IntegerSequence<std::size_t>
    ) noexcept
    {
        return {{nominatorSymbol[IndicesNom]..., '\0'}};
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
            MakeIntegerSequence<std::size_t, 0, nomLength - 1>{},
            SymbolDenominators::get(),
            MakeIntegerSequence<std::size_t, 0, denomLength - 1>{}
        );
    }
};

/* ************************************************************************ */

/**
 * @brief Symbol for specified unit type.
 *
 * @tparam UnitType
 */
template<typename UnitType>
struct Symbol;

/* ************************************************************************ */

/**
 * @brief Symbol for specified unit type.
 *
 * @tparam Nom   List type.
 * @tparam Denom List type.
 */
template<typename Nom, typename Denom>
struct Symbol<Unit<Nom, Denom>>
{
    /// Unit symbol type.
    using type = SymbolBase<Nom, Denom>;

    /// Symbol for nominators.
    using nominators = typename type::SymbolNominators;

    /// Symbol for denominators.
    using denominators = typename type::SymbolDenominators;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
