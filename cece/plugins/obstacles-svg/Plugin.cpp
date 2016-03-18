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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// C++
#include <iterator>
#include <algorithm>

// CeCe
#include "cece/core/DynamicArray.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Color.hpp"
#endif

// NanoSVG
#define NANOSVG_IMPLEMENTATION  // Expands implementation
#include "nanosvg/nanosvg.h"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Calculate distance of point segment.
 *
 * @param v
 * @param pv
 * @param pq
 *
 * @return
 */
float distPtSegSq(Vector<float> v, Vector<float> vp, Vector<float> vq)
{
    const auto vpq = vq - vp;
    const auto dist = vpq.getLengthSquared();
    auto dv = v - vp;
    auto t = vpq.dot(dv);

    if (dist > 0)
        t /= dist;

    if (t < 0)
        t = 0;
    else if (t > 1)
        t = 1;

    dv = vp + t * vpq - v;

    return dv.getLengthSquared();
}

/* ************************************************************************ */

/**
 * @brief Store cubic bezier curve.
 *
 * @param out
 * @parma
 */
template<typename Fn>
void cubicBezier(Fn store, StaticArray<Vector<float>, 4> vs, float tol, int level = 0)
{
    // Limit recursion
    if (level > 12)
        return;

    Log::debug("0: ", vs[0].getX(), "; ", vs[0].getY());
    Log::debug("1: ", vs[1].getX(), "; ", vs[1].getY());
    Log::debug("2: ", vs[2].getX(), "; ", vs[2].getY());
    Log::debug("3: ", vs[3].getX(), "; ", vs[3].getY());

    const auto v0    = vs[0];
    const auto v1    = vs[1];
    const auto v2    = vs[2];
    const auto v3    = vs[3];
    const auto v01   = (v0 + v1) * 0.5f;
    const auto v12   = (v1 + v2) * 0.5f;
    const auto v23   = (v2 + v3) * 0.5f;
    const auto v012  = (v01 + v12) * 0.5f;
    const auto v123  = (v12 + v23) * 0.5f;
    const auto v0123 = (v012 + v123) * 0.5f;

    auto dSq = distPtSegSq(v0123, v0, v3);

    if (dSq > tol * tol)
    {
        cubicBezier(store, {v0, v01, v012, v0123}, tol, level + 1);
        cubicBezier(store, {v0123, v123, v23, v3}, tol, level + 1);
    }
    else
    {
        store(vs[3]);
    }
}

/* ************************************************************************ */

/**
 * @brief Store path to vertices.
 *
 * @param out
 * @param npts
 * @param closed
 * @param tol
 */
template<typename Fn>
void storePath(Fn store, NSVGpath* path, float tol)
{
    // Store first vector
    //store(Vector<float>{path->pts[0], path->pts[1]});

    for (int i = 0; i < path->npts - 1; i += 3)
    {
        float* p = &(path->pts[i * 2]);
        cubicBezier(store, {
            Vector<float>{p[0], p[1]},
            Vector<float>{p[2], p[3]},
            Vector<float>{p[4], p[5]},
            Vector<float>{p[6], p[7]}
        }, tol, 0);
    }

    // Store closing vector
    //if (path->closed)
    //    store(Vector<float>{path->pts[0], path->pts[1]});
}

/* ************************************************************************ */

}

/* ************************************************************************ */

class ObstaclesSvgApi : public plugin::Api
{

    /**
     * @brief Returns a list of required plugins.
     *
     * @return
     */
    DynamicArray<String> requiredPlugins() const override
    {
        return {"obstacle"};
    }


    /**
     * @brief Load plugin configuration.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    void loadConfig(simulator::Simulation& simulation, const config::Configuration& config) override
    {
        const float px = 0.3;
        const auto file = simulation.getResource(config.get("filename"));

        if (!file)
            throw RuntimeException("SVG file not found");

        DynamicArray<char> content;
        content.reserve(2048);

        *file >> std::noskipws;
        std::copy(std::istream_iterator<char>(*file), std::istream_iterator<char>(),
            std::back_inserter(content));

        // SVG image
        NSVGimage* image = nsvgParse(content.data(), "px", 96);

        if (!image)
            throw RuntimeException("Cannot parse SVG");

        auto obstacle = simulation.buildObject("obstacle.Polygon");
        auto& shapes = obstacle->getMutableShapes();

#ifdef CECE_ENABLE_RENDER
        obstacle->setColor(config.get("color", obstacle->getColor()));
#endif

        const auto size = Vector<float>(image->width, image->height);

        // Coordinate scale
        const auto scale = simulation.getWorldSize() / size;
        const auto offset = size * 0.5;

        // Foreach shapes
        for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next)
        {
            ShapeEdges oShape;

            // Foreach paths
            for (NSVGpath* path = shape->paths; path != nullptr; path = path->next)
            {
                storePath([&oShape, &offset, &scale] (Vector<float> vec) {
                    const auto v = (vec - offset) * scale * Vector<float>(1, -1);
                    if (oShape.edges.empty() ||
                        oShape.edges.back().distanceSquared(v).value() > 0.05)
                    {
                        oShape.edges.push_back(v);
                        Log::debug(v.getX(), "; ", v.getY());
                    }
                }, path, px * 1.5);
            }

            shapes.push_back(std::move(oShape));
        }

        obstacle->initShapes();
    }


    /**
     * @brief Store plugin configuration.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    void storeConfig(const simulator::Simulation& simulation, config::Configuration& config) const override
    {
        // TODO: implement
    }
};

/* ************************************************************************ */

CECE_DEFINE_PLUGIN(obstacles_svg, ObstaclesSvgApi)

/* ************************************************************************ */
