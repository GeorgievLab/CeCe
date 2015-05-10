
/* ************************************************************************ */

// C++
#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <array>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <iomanip>

/* ************************************************************************ */

/**
 * @brief Error function.
 *
 * @param err Error message.
 */
[[noreturn]] void error(const std::string& err)
{
    std::cerr << err << std::endl;
    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Convert code into C/C++ array sequence.
 *
 * @param out
 * @param symbol
 * @param src
 */
static void convert(std::ostream& out, const std::string& symbol, std::istream& src)
{
    unsigned int valueCounter = 0;

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::hash<std::string> hasher;

    std::string code;
    std::copy(std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>(), std::back_inserter(code));

    out << "#include <array>\n";
    out << "\n";
    out << "// " << std::put_time(&tm, "%F %T %Z") << "\n";
    out << "// #" << hasher(code) << "\n";
    out << "static const std::array<char, " << code.length() + 1 << "> " << symbol << " = {\n  ";

    for (auto c : code)
    {
        out << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << ", ";

        if (++valueCounter >= 8)
        {
            valueCounter = 0;
            out << "\n  ";
        }
    }

    out << "0x00\n};\n";
}

/* ************************************************************************ */

/**
 * @brief Split source with multiple shaders into separate ones.
 *
 * @param src
 *
 * @return
 */
std::array<std::string, 2> splitShaders(std::istream& src)
{
    std::array<std::string, 2> shaders;
    shaders[0].reserve(1024);
    shaders[1].reserve(1024);

    std::string line;

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
        if (argc < 5)
        {
            error("not enough arguments: <mode>{default|shaders} <symbol> [<symbol2>] <file> <outfile> [<outfile2>]");
        }

        const std::string mode = argv[1];

        if (mode == "shaders")
        {
            if (argc < 7)
            {
                error("not enough arguments: shaders <symbol1> <symbol2> <file> <outfile1> <outfile2>");
            }

            const std::string symbol1 = argv[2];
            const std::string symbol2 = argv[3];
            const std::string filename = argv[4];
            const std::string outFilename1 = argv[5];
            const std::string outFilename2 = argv[6];

            std::ifstream file(filename, std::ios::in | std::ios::binary);
            auto sources = splitShaders(file);

            {
                std::istringstream is(sources[0]);
                std::ofstream outFile(outFilename1, std::ios::out | std::ios::binary);
                convert(outFile, symbol1, is);
            }

            {
                std::istringstream is(sources[1]);
                std::ofstream outFile(outFilename2, std::ios::out | std::ios::binary);
                convert(outFile, symbol2, is);
            }
        }
        else if (mode == "default")
        {
            const std::string symbol = argv[2];
            const std::string filename = argv[3];
            const std::string outFilename = argv[4];

            std::ifstream file(filename, std::ios::in | std::ios::binary);
            std::ofstream outFile(outFilename, std::ios::out | std::ios::binary);

            // Convert file
            convert(outFile, symbol, file);
        }
        else
        {
            throw std::runtime_error("Unknown mode: " + mode);
        }
    }
    catch (const std::exception& e)
    {
        error(e.what());
    }
}

/* ************************************************************************ */
