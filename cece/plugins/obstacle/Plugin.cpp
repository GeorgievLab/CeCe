/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// CeCe
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/plugin/Repository.hpp"

// Plugin
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Polygon.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::plugin::obstacle;

/* ************************************************************************ */

/**
 * @brief Obstacles plugin API.
 */
class ObstacleApi : public plugin::Api
{

    /**
     * @brief On plugin load.
     *
     * @param repository Plugin repository.
     */
    void onLoad(plugin::Repository& repository) const override
    {
        repository.registerApi(this).
            registerObject<Circle>("obstacle.Circle").
            registerObject<Rectangle>("obstacle.Rectangle").
            registerObject<Polygon>("obstacle.Polygon")
        ;
    }

};

/* ************************************************************************ */

CECE_DEFINE_PLUGIN(obstacle, ObstacleApi)

/* ************************************************************************ */
