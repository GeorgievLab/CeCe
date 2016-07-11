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
#include <QWidget>
#include <QStringList>
#include <QList>
#include <QVariant>

/* ************************************************************************ */

namespace Ui { class PlotWidget; }

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief Widget for plot.
 */
class PlotWidget : public QWidget
{
    Q_OBJECT

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     */
    explicit PlotWidget(QWidget *parent = 0);


    /**
     * @brief Destructor.
     */
    ~PlotWidget();


// Public Mutators
public:


    /**
     * @brief Set column name for X axis.
     * @param name
     */
    void setXAxisName(QString name) noexcept;


    /**
     * @brief Set column name for Y axis.
     * @param name
     */
    void setYAxisName(QString name) noexcept;


    /**
     * @brief Set column name for group.
     * @param name
     */
    void setGroupName(QString name) noexcept;


    /**
     * @brief Set plot type.
     * @param type
     */
    void setType(QString type) noexcept;


// Slots
public slots:


    /**
     * @brief Add plot data.
     *
     * @param names
     * @param values
     */
    void dataAdd(QStringList names, QList<QVariant> value);


// Private Data Members
private:

    /// UI.
    Ui::PlotWidget *ui;

    /// Name of X axis.
    QString m_xName;

    /// Name of Y axis.
    QString m_yName;

    /// Name of group.
    QString m_groupName;

    /// Plot type.
    QString m_type;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
