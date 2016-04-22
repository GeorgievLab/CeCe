:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::
:: Georgiev Lab (c) 2015-2016                                                ::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::
:: Department of Cybernetics                                                 ::
:: Faculty of Applied Sciences                                               ::
:: University of West Bohemia in Pilsen                                      ::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::
::                                                                           ::
:: This file is part of CeCe.                                                ::
::                                                                           ::
:: CeCe is free software: you can redistribute it and/or modify              ::
:: it under the terms of the GNU General Public License as published by      ::
:: the Free Software Foundation, either version 3 of the License, or         ::
:: (at your option) any later version.                                       ::
::                                                                           ::
:: CeCe is distributed in the hope that it will be useful,                   ::
:: but WITHOUT ANY WARRANTY; without even the implied warranty of            ::
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             ::
:: GNU General Public License for more details.                              ::
::                                                                           ::
:: You should have received a copy of the GNU General Public License         ::
:: along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             ::
::                                                                           ::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::

@echo on

md vendor
pushd vendor

:: Clone dependencies
git clone https://github.com/erincatto/Box2D.git -b v2.3.1 Box2D
git clone https://github.com/madler/zlib.git -b v1.2.8 zlib
git clone https://github.com/glennrp/libpng -b libpng16 libpng
git clone https://github.com/glfw/glfw.git -b 3.1.2 glfw3

:: Set MinGW path
SET PATH=%PATH:C:\Program Files\Git\usr\bin;=%
SET PATH=C:\MinGW\bin;%PATH%

:: Build boost with MinGW
pushd C:\Libraries\boost
call bootstrap.bat mingw
b2 toolset=gcc --with-filesystem --with-system variant=release
popd

:: Box2D
pushd Box2D\Box2D

md build
pushd build

:: Configure
cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=release -DBOX2D_INSTALL=On -DBOX2D_BUILD_EXAMPLES=Off -DBOX2D_BUILD_SHARED=On -DBOX2D_BUILD_STATIC=Off .. || EXIT /B 1

:: Build
cmake --build . || EXIT /B 1

:: Install
mingw32-make install || EXIT /B 1

popd
popd

::::::::::::::::

:: Zlib
pushd zlib

md build
pushd build

:: Configure
cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=release .. || EXIT /B 1

:: Build
cmake --build . || EXIT /B 1

:: Install
mingw32-make install || EXIT /B 1

popd
popd

::::::::::::::::

:: LibPNG
pushd libpng

md build
pushd build

:: Configure
cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=release .. || EXIT /B 1

:: Build
cmake --build . || EXIT /B 1

:: Install
mingw32-make install || EXIT /B 1

popd
popd

::::::::::::::::

:: GLFW3
pushd glfw3

md build
pushd build

:: Configure
cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=release -DGLFW_BUILD_DOCS=Off -DGLFW_BUILD_EXAMPLES=Off -DGLFW_BUILD_TESTS=Off .. || EXIT /B 1

:: Build
cmake --build . || EXIT /B 1

:: Install
mingw32-make install || EXIT /B 1

popd
popd

:: vendor
popd

@echo off

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::

