# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
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

set(SRCS
    Macro.hpp
    constants.hpp
    fastexp.hpp
    DynamicArray.hpp
    StaticArray.hpp
    StaticMatrix.hpp
    Unit.hpp
    Unit.cpp
    UnitIo.hpp
    UnitIo.cpp
    UnitSymbol.hpp
    Units.hpp
    Units.cpp
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
    CsvFile.hpp
    CsvFile.cpp
    DataExport.hpp
    DataExport.cpp
    DataExportCsv.hpp
    DataExportCsv.cpp
    DataExportFactory.hpp
    DataExportFactory.cpp
    DataExportCsvFactory.hpp
    DataExportCsvFactory.cpp
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
    IterationType.hpp
    IterationRange.hpp
    CliColor.hpp
    CliColor.cpp
)

set(SRCS_TEST
    test/AlignedAllocatorTest.cpp
    test/DynamicArrayTest.cpp
    test/IteratorRangeTest.cpp
    test/VectorRangeTest.cpp
    test/UnitsTest.cpp
    test/StringViewTest.cpp
    test/VectorTest.cpp
    test/VectorUnitsTest.cpp
    test/ExpressionParserTest.cpp
    test/TokenizerTest.cpp
    test/ParametersTest.cpp
    test/PtrContainerTest.cpp
    test/ViewPtrTest.cpp
)

# ######################################################################### #

dir_pretend(SOURCES core/ ${SRCS})
dir_pretend(SOURCES_TEST core/ ${SRCS_TEST})

set(SOURCES_CORE ${SOURCES} PARENT_SCOPE)
set(SOURCES_CORE_TEST ${SOURCES_TEST} PARENT_SCOPE)

# ######################################################################### #

# Add tests
build_test(core
    SOURCES
        ${SRCS_TEST}
        ${SRCS}
    LIBRARIES
        ${Boost_LIBRARIES}
)

# ######################################################################### #
