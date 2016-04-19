# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

# Create project
project(core CXX)

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME} OBJECT
    Macro.hpp
    constants.hpp
    DynamicArray.hpp
    StaticArray.hpp
    StaticMatrix.hpp
    Unit.hpp
    Unit.cpp
    UnitIo.hpp
    UnitIo.cpp
    UnitSymbol.hpp
    Units.hpp
    UnitsCtors.hpp
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
    InOutStream.hpp
    FileStream.hpp
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
    StringStream.hpp
    Tokenizer.hpp
    Real.hpp
    Mutex.hpp
    Pair.hpp
    Variant.hpp
    DataTable.hpp
    DataTable.cpp
    SharedPtr.hpp
    TriBool.hpp
    Parameters.hpp
    Parameters.cpp
    Factory.hpp
    FactoryManager.hpp
    Shape.hpp
    ShapeToGrid.hpp
    PtrContainer.hpp
    PtrNamedContainer.hpp
)

# ######################################################################### #

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE On)

# ######################################################################### #

# Install header files
install(DIRECTORY .
    DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
    COMPONENT Headers
    FILES_MATCHING PATTERN "*.hpp"
)

# ######################################################################### #

# Add tests
build_test(core
    SOURCES
        test/DynamicArrayTest.cpp
        test/IteratorRangeTest.cpp
        test/VectorRangeTest.cpp
        test/UnitsTest.cpp
        test/StringViewTest.cpp
        test/VectorTest.cpp
        test/VectorUnitsTest.cpp
        test/ExpressionParserTest.cpp
        test/TokenizerTest.cpp
        ExpressionParser.hpp
        ExpressionParser.cpp
        Unit.hpp
        Unit.cpp
        UnitIo.hpp
        UnitIo.cpp
        UnitSymbol.hpp
        Units.hpp
        UnitsCtors.hpp
        Parameters.hpp
        Parameters.cpp
)

# ######################################################################### #
