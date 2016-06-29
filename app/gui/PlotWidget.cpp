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

    ui->plot->legend->setVisible(true);
}

/* ************************************************************************ */

PlotWidget::~PlotWidget()
{
    delete ui;
}

/* ************************************************************************ */

void PlotWidget::setXAxisLabel(QString label) noexcept
{
    ui->plot->xAxis->setLabel(label);
}

/* ************************************************************************ */

void PlotWidget::setYAxisLabel(QString label) noexcept
{
    ui->plot->yAxis->setLabel(label);
}

/* ************************************************************************ */

void PlotWidget::dataAdd(double x, double y, QString color)
{
    QCPGraph* graph = nullptr;

    for (int i = 0; i < ui->plot->graphCount(); ++i)
    {
        if (ui->plot->graph(i)->name() == color)
        {
            graph = ui->plot->graph(i);
            break;
        }
    }

    if (!graph)
    {
        const int i = ui->plot->graphCount();

        static const QColor colors[4] = {
            Qt::red,
            Qt::green,
            Qt::blue,
            Qt::yellow
        };

        graph = ui->plot->addGraph();
        graph->setName(color);
        graph->setPen(QPen(colors[i % 4]));
    }

    graph->addData(x, y);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
