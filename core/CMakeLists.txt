# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8.8)

# ######################################################################### #

# Create project
project(core CXX)

# ######################################################################### #

# Current version is unable to add object library into shared library.
if (UNIX)
    add_compile_options(-fPIC)
endif ()

# DLL export
if (WIN32)
    add_definitions(-DSIMULATOR_DLL_EXPORT=1)
endif ()

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME} OBJECT
    compatibility.hpp
    constants.hpp
    DynamicArray.hpp
    StaticArray.hpp
    StaticMatrix.hpp
    Units.hpp
    Units.cpp
    Vector.hpp
    Vector.cpp
    VectorUnits.hpp
    VectorUnits.cpp
    Grid.hpp
    Grid.cpp
    AlignedAllocator.hpp
    AlignedAllocator.cpp
    ExpressionParser.hpp
    ExpressionParser.cpp
    Log.hpp
    Log.cpp
    TimeMeasurement.hpp
    TimeMeasurement.cpp
    String.hpp
    StringView.hpp
    FilePath.hpp
    FilePath.cpp
    Exception.hpp
    OutStream.hpp
    InStream.hpp
    ValueIterator.hpp
    Range.hpp
    VectorRange.hpp
    Any.hpp
    IntegerSequence.hpp
    Tuple.hpp
    UniquePtr.hpp
    ViewPtr.hpp
    ListenerContainer.hpp
    Zero.hpp
)

# ######################################################################### #

# Add tests
build_test(core
    SOURCES
        test/test.cpp
        test/DynamicArrayTest.cpp
        test/IteratorRangeTest.cpp
        test/VectorRangeTest.cpp
        test/UnitsTest.cpp
        test/StringViewTest.cpp
        test/ExpressionParserTest.cpp
        ExpressionParser.hpp
        ExpressionParser.cpp
)

# ######################################################################### #
