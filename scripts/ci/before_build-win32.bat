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

:: Shared configuration arguments
:: SET ARGS=-DCMAKE_BUILD_TYPE=release -DDEV_TESTS_BUILD=On -DDEV_PHYSICS_BUILTIN_DEBUG=On -DDEV_PLUGIN_streamlines_RENDER=On -DPNG_PNG_INCLUDE_DIR="C:\Program Files (x86)\libpng\include" -DPNG_LIBRARY_RELEASE="C:\Program Files (x86)\libpng\lib\libpng16.a"
SET ARGS=-DCMAKE_BUILD_TYPE=release -DCONFIG_CLI_ENABLE_VIDEO_CAPTURE=Off -DDEV_PHYSICS_BUILTIN_DEBUG=On -DDEV_PLUGIN_streamlines_RENDER=On -DPNG_PNG_INCLUDE_DIR="C:\Program Files (x86)\libpng\include" -DPNG_LIBRARY_RELEASE="C:\Program Files (x86)\libpng\lib\libpng16.a"

md build
pushd build

:: Configure project
cmake -G "%GENERATOR%" %ARGS% .. || EXIT /B 1

popd

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ::

