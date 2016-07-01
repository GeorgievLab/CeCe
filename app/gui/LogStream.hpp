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

#pragma once

/* ************************************************************************ */

// Qt
#include <QObject>

// CeCe
#include "cece/core/Log.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief Log output stream.
 */
class LogStream : public QObject, public Log::Output
{
    Q_OBJECT


// Public Signals
signals:


    /**
     * @brief A new message logged.
     *
     * @param type    Message type.
     * @param section Message section.
     * @param msg     Message to log.
     */
    void append(Log::Type type, QString section, QString message);


// Public Operations
public:


    /**
     * @brief Write a message to output.
     *
     * @param type    Message type.
     * @param section Message section.
     * @param msg     Message to log.
     */
    void write(Log::Type type, const String& section, const String& msg) override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
