/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

namespace cece {
namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

/**
 * @brief Functor Signum
 *
 */
template <typename T>
struct Signum
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &fa) const
    {
        return (fa > 0) - (fa < 0);
    }
};

/**
 * @brief Functor Equal
 *
 */
template <typename T>
struct Equal
{
    using result_type = bool;
    using first_argument_type = T;
    using second_argument_type = T;

    constexpr bool operator()(const T &fa, const T &sa) const
    {
        return std::fabs(fa - sa) < std::numeric_limits<T>::epsilon();
    }
};

/**
 * @brief Functor NotEqual
 *
 */
template <typename T>
struct NotEqual
{
    using result_type = bool;
    using first_argument_type = T;
    using second_argument_type = T;

    constexpr bool operator()(const T &fa, const T &sa) const
    {
        return !Equal<T>{}(fa, sa);
    }
};

/**
 * @brief Functor GreaterEqual
 *
 */
template <typename T>
struct GreaterEqual
{
    using result_type = bool;
    using first_argument_type = T;
    using second_argument_type = T;

    constexpr bool operator()(const T &fa, const T &sa) const
    {
        return std::greater<T>{}(fa, sa) || Equal<T>{}(fa, sa);
    }
};

/**
 * @brief Functor LessEqual
 *
 */
template <typename T>
struct LessEqual
{
    using result_type = bool;
    using first_argument_type = T;
    using second_argument_type = T;

    constexpr bool operator()(const T &fa, const T &sa) const
    {
        return std::less<T>{}(fa, sa) || Equal<T>{}(fa, sa);
    }
};

/**
 * @brief Functor hill(x)
 *
 */
template <typename T>
struct Hill
{
    using result_type = T;
    using first_argument_type = T;
    using second_argument_type = T;
    using third_argument_type = T;

    constexpr T operator()(const T &fa, const T &sa, const T &ta) const
    {
        return std::pow(fa, ta) / (std::pow(fa, ta) + std::pow(sa, ta));
    }
};

/**
 * @brief Functor abs(x)
 *
 */
template <typename T>
struct Abs
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::abs(root);
    }
};

/**
 * @brief Functor gamma(x)
 *
 */
template <typename T>
struct Gamma
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::tgamma(root);
    }
};

/**
 * @brief Functor x^y
 *
 */
template <typename T>
struct Pow
{
    using result_type = T;
    using first_argument_type = T;
    using second_argument_type = T;

    constexpr T operator()(const T &lhs, const T &rhs) const
    {
        return std::pow(lhs, rhs);
    }
};

/**
 * @brief Functor logx(y)
 *
 */
template <typename T>
struct Log
{
    using result_type = T;
    using first_argument_type = T;
    using second_argument_type = T;

    constexpr T operator()(const T &lhs, const T &rhs) const
    {
        return std::log(rhs) / std::log(lhs);
    }
};

/**
 * @brief Functor e^x
 *
 */
template <typename T>
struct Exp
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::exp(root);
    }
};

/**
 * @brief Functor ln(x)
 *
 */
template <typename T>
struct Ln
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::log(root);
    }
};

/**
 * @brief Functor log10(x)
 *
 */
template <typename T>
struct Log10
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::log10(root);
    }
};

/**
 * @brief Functor log2(x)
 *
 */
template <typename T>
struct Log2
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::log2(root);
    }
};

/**
 * @brief Functor sqrt(y)
 *
 */
template <typename T>
struct Sqrt
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::sqrt(root);
    }
};

/**
 * @brief Functor cbrt(y)
 *
 */
template <typename T>
struct Cbrt
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::cbrt(root);
    }
};

/**
 * @brief Functor sin(x)
 *
 */
template <typename T>
struct Sine
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::sin(root);
    }
};

/**
 * @brief Functor cos(x)
 *
 */
template <typename T>
struct Cosine
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::cos(root);
    }
};

/**
 * @brief Functor tan(x)
 *
 */
template <typename T>
struct Tangent
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::tan(root);
    }
};

/**
 * @brief Functor cot(x)
 *
 */
template <typename T>
struct Cotangent
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return 1 / std::tan(root);
    }
};

/**
 * @brief Functor sec(x)
 *
 */
template <typename T>
struct Secant
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return 1 / std::cos(root);
    }
};

/**
 * @brief Functor csc(x)
 *
 */
template <typename T>
struct Cosecant
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return 1 / std::sin(root);
    }
};

/**
 * @brief Functor asin(x)
 *
 */
template <typename T>
struct Arcsine
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::asin(root);
    }
};

/**
 * @brief Functor acos(x)
 *
 */
template <typename T>
struct Arccosine
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::acos(root);
    }
};

/**
 * @brief Functor atan(x)
 *
 */
template <typename T>
struct Arctangent
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::atan(root);
    }
};

/**
 * @brief Functor acot(x)
 *
 */
template <typename T>
struct Arccotangent
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::atan(1 / root);
    }
};

/**
 * @brief Functor asec(x)
 *
 */
template <typename T>
struct Arcsecant
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::acos(1 / root);
    }
};

/**
 * @brief Functor acsc(x)
 *
 */
template <typename T>
struct Arccosecant
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::asin(1 / root);
    }
};

/**
 * @brief Functor sinh(x)
 *
 */
template <typename T>
struct Sinh
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::sinh(root);
    }
};

/**
 * @brief Functor cosh(x)
 *
 */
template <typename T>
struct Cosh
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::cosh(root);
    }
};

/**
 * @brief Functor tanh(x)
 *
 */
template <typename T>
struct Tanh
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::tanh(root);
    }
};

/**
 * @brief Functor coth(x)
 *
 */
template <typename T>
struct Coth
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return 1 / std::tanh(root);
    }
};

/**
 * @brief Functor sech(x)
 *
 */
template <typename T>
struct Sech
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return 1 / std::cosh(root);
    }
};

/**
 * @brief Functor csch(x)
 *
 */
template <typename T>
struct Csch
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return 1 / std::sinh(root);
    }
};

/**
 * @brief Functor asinh(x)
 *
 */
template <typename T>
struct Arcsinh
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::asinh(root);
    }
};

/**
 * @brief Functor acosh(x)
 *
 */
template <typename T>
struct Arccosh
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::acosh(root);
    }
};

/**
 * @brief Functor atanh(x)
 *
 */
template <typename T>
struct Arctanh
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::atanh(root);
    }
};

/**
 * @brief Functor acoth(x)
 *
 */
template <typename T>
struct Arccoth
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::atanh(1 / root);
    }
};

/**
 * @brief Functor asech(x)
 *
 */
template <typename T>
struct Arcsech
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::acosh(1 / root);
    }
};

/**
 * @brief Functor acsch(x)
 *
 */
template <typename T>
struct Arccsch
{
    using result_type = T;
    using argument_type = T;

    constexpr T operator()(const T &root) const
    {
        return std::asinh(1 / root);
    }
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
