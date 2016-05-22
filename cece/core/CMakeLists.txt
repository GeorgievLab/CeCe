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

# Create library
set(SOURCES_CORE
    core/Macro.hpp
    core/constants.hpp
    core/DynamicArray.hpp
    core/StaticArray.hpp
    core/StaticMatrix.hpp
    core/Unit.hpp
    core/Unit.cpp
    core/UnitIo.hpp
    core/UnitIo.cpp
    core/UnitSymbol.hpp
    core/Units.hpp
    core/Units.cpp
    core/UnitsCtors.hpp
    core/Vector.hpp
    core/Vector.cpp
    core/VectorUnits.hpp
    core/VectorUnits.cpp
    core/Grid.hpp
    core/Grid.cpp
    core/AlignedAllocator.hpp
    core/AlignedAllocator.cpp
    core/ExpressionParser.hpp
    core/ExpressionParser.cpp
    core/Log.hpp
    core/Log.cpp
    core/TimeMeasurement.hpp
    core/TimeMeasurement.cpp
    core/String.hpp
    core/StringView.hpp
    core/FilePath.hpp
    core/FilePath.cpp
    core/Exception.hpp
    core/OutStream.hpp
    core/InStream.hpp
    core/InOutStream.hpp
    core/FileStream.hpp
    core/CsvFile.hpp
    core/CsvFile.cpp
    core/ValueIterator.hpp
    core/Range.hpp
    core/VectorRange.hpp
    core/Any.hpp
    core/IntegerSequence.hpp
    core/Tuple.hpp
    core/UniquePtr.hpp
    core/ViewPtr.hpp
    core/ListenerContainer.hpp
    core/Zero.hpp
    core/StringStream.hpp
    core/Tokenizer.hpp
    core/Real.hpp
    core/Mutex.hpp
    core/Pair.hpp
    core/Variant.hpp
    core/DataTable.hpp
    core/DataTable.cpp
    core/SharedPtr.hpp
    core/TriBool.hpp
    core/Parameters.hpp
    core/Parameters.cpp
    core/Factory.hpp
    core/FactoryManager.hpp
    core/Shape.hpp
    core/ShapeToGrid.hpp
    core/PtrContainer.hpp
    core/PtrNamedContainer.hpp
    PARENT_SCOPE
)

# ######################################################################### #

# Add tests
build_test(core
    SOURCES
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
        ${SOURCES_CORE}
    LIBRARIES
        ${Boost_LIBRARIES}
)

# ######################################################################### #
