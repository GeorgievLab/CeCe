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

# Options specific for different compilers
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    option(CLANG_ASAN               "If address sanitizer should be used" Off)
    option(CLANG_MSAN               "If memory sanitizer should be used" Off)
    option(CLANG_USE_LIBCPP         "If libc++ should be used" Off)

    # C++11 required
    #add_compile_options(-Wall -pedantic-errors -pedantic -std=c++11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors -pedantic -std=c++11")

    # Use libc++
    if (CLANG_USE_LIBCPP)
        #add_compile_options(-stdlib=libc++)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    endif()

    # Debug flags
    if (CLANG_ASAN)
        message(STATUS "Using address sanitizer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    elseif (CLANG_MSAN)
        message(STATUS "Using memory sanitizer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCXX)
    # using GCC

    # C++11 required
    #add_compile_options(-Wall -pedantic-errors -pedantic -std=c++11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors -pedantic -std=c++11")

    # Static linkage
    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    endif ()

elseif (MSVC)
    # using Visual Studio C++
    message(FATAL_ERROR
        "Visual Studio Compiler doesn't support C++11 standard which is "
        "required, so we don't support Visual Studio Compiler."
    )
endif()

# ######################################################################### #

