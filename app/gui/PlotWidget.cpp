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

// Declaration
#include "PlotWidget.hpp"

// Ui
#include "ui_PlotWidget.h"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
}

/* ************************************************************************ */

PlotWidget::~PlotWidget()
{
    delete ui;
}

/* ************************************************************************ */

void PlotWidget::setXAxisName(QString name) noexcept
{
    ui->plot->xAxis->setLabel(name);
    m_xName = name;
}

/* ************************************************************************ */

void PlotWidget::setYAxisName(QString name) noexcept
{
    ui->plot->yAxis->setLabel(name);
    m_yName = name;
}

/* ************************************************************************ */

void PlotWidget::setGroupName(QString name) noexcept
{
    ui->plot->legend->setVisible(true);
    m_groupName = name;
}

/* ************************************************************************ */

void PlotWidget::setType(QString type) noexcept
{
    m_type = type;
}

/* ************************************************************************ */

void PlotWidget::dataAdd(QStringList names, QList<QVariant> values)
{
    static const QColor colors[4] = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::yellow
    };


    double xValue = 0;
    double yValue = 0;
    QString group;

    // Foreach values
    for (int i = 0; i < names.size(); ++i)
    {
        if (names[i] == m_xName)
            xValue = values[i].toDouble();
        else if (names[i] == m_yName)
            yValue = values[i].toDouble();
        else if (names[i] == m_groupName)
            group = values[i].toString();
    }

    QCPGraph* graph = nullptr;

    // Find graph by name
    for (int i = 0; i < ui->plot->graphCount(); ++i)
    {
        if (ui->plot->graph(i)->name() == group)
        {
            graph = ui->plot->graph(i);
            break;
        }
    }

    // No graph for given group
    if (!graph)
    {
        const int i = ui->plot->graphCount();
        graph = ui->plot->addGraph();
        graph->setName(group);
        graph->setPen(QPen(colors[i % 4]));

        if (m_type == "Point")
        {
            graph->setLineStyle(QCPGraph::lsNone);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
        }
    }

    graph->addData(xValue, yValue);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
