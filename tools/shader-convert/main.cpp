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

// C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <iomanip>
#include <locale>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/String.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

/**
 * @brief Error function.
 *
 * @param err Error message.
 */
[[noreturn]] void error(const String& err)
{
    std::cerr << err << std::endl;
    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Put time.
 *
 * @param out
 * @param tmb
 * @param fmt
 *
 * @return
 */
template<typename CharT, typename Traits>
void put_time(std::basic_ostream<CharT, Traits>& out, const std::tm* tmb, const CharT* fmt)
{
    typedef std::ostreambuf_iterator<CharT, Traits> Iter;
    typedef std::time_put<CharT> TimePut;
    const TimePut& tp = std::use_facet<TimePut>(out.getloc());
    const Iter end = tp.put(Iter(out.rdbuf()), out, out.fill(), tmb, fmt, fmt + Traits::length(fmt));
    if (end.failed())
        out.setstate(std::ios_base::badbit);
}

/* ************************************************************************ */

/**
 * @brief Convert code into C/C++ array sequence.
 *
 * @param out
 * @param symbol
 * @param src
 */
static void convert(OutStream& out, const String& symbol, InStream& src)
{
    unsigned int valueCounter = 0;

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::hash<String> hasher;

    String code;
    std::copy(std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>(), std::back_inserter(code));

    out << "#include \"cece/core/StaticArray.hpp\"\n";
    out << "\n";
    out << "// ";
    put_time(out, &tm, "%F %T %Z");
    out << "\n";
    out << "// #" << hasher(code) << "\n";
    out << "static const cece::StaticArray<unsigned char, " << code.length() + 1 << "> " << symbol << " = {{\n  ";

    for (auto c : code)
    {
        out << "0x" << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(static_cast<unsigned char>(c)) << ", ";

        if (++valueCounter >= 8)
        {
            valueCounter = 0;
            out << "\n  ";
        }
    }

    out << "0x00\n}};\n";
}

/* ************************************************************************ */

/**
 * @brief Split source with multiple shaders into separate ones.
 *
 * @param src
 *
 * @return
 */
StaticArray<String, 2> splitShaders(InStream& src)
{
    StaticArray<String, 2> shaders;
    shaders[0].reserve(1024);
    shaders[1].reserve(1024);

    String line;

    auto readRest = [&](int id) {
        while (std::getline(src, line))
        {
            if (!line.empty() && line[0] == '[')
                break;

            shaders[id].append(line).push_back('\n');
        }
    };

    while (std::getline(src, line))
    {
        if (line == "[VertexShader]")
        {
            readRest(0);
        }

        if (line == "[FragmentShader]")
        {
            readRest(1);
        }

        // Ignore lines
    }

    return shaders;
}

/* ************************************************************************ */

/**
 * @brief Entry function.
 *
 * @param argc
 * @param argv
 */
int main(int argc, char** argv)
{
    try
    {
        // Parse parameters
        if (argc < 4)
            error("not enough arguments: <shader-file> <out-dir> <name>");

        const String inFilename = argv[1];
        const String outDir = argv[2];
        const String name = argv[3];

        std::ifstream inFile(inFilename, std::ios::in | std::ios::binary);
        auto sources = splitShaders(inFile);

        {
            String outFilename = outDir + "/vs." + name + ".hpp";
            std::istringstream is(sources[0]);
            std::ofstream outFile(outFilename, std::ios::out | std::ios::binary);
            std::cout << "Vertex shader: " << outFilename << std::endl;
            convert(outFile, "g_vertexShader", is);
        }

        {
            String outFilename = outDir + "/fs." + name + ".hpp";
            std::istringstream is(sources[1]);
            std::ofstream outFile(outFilename, std::ios::out | std::ios::binary);
            std::cout << "Fragment shader: " << outFilename << std::endl;
            convert(outFile, "g_fragmentShader", is);
        }
    }
    catch (const Exception& e)
    {
        error(e.what());
    }
}

/* ************************************************************************ */
