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
#include "PlotCustomizationDialog.hpp"

// UI
#include "ui_PlotCustomizationDialog.h"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

PlotCustomizationDialog::PlotCustomizationDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PlotCustomizationDialog)
{
    ui->setupUi(this);
}

/* ************************************************************************ */

PlotCustomizationDialog::~PlotCustomizationDialog()
{
    delete ui;
}

/* ************************************************************************ */

QString PlotCustomizationDialog::getPlotTitle() const noexcept
{
    return ui->lineEditTitle->text();
}

/* ************************************************************************ */

void PlotCustomizationDialog::setPlotTitle(QString title) noexcept
{
    ui->lineEditTitle->setText(title);
}

/* ************************************************************************ */

QString PlotCustomizationDialog::getPlotXLabel() const noexcept
{
    return ui->lineEditX->text();
}

/* ************************************************************************ */

void PlotCustomizationDialog::setPlotXLabel(QString label) noexcept
{
    ui->lineEditX->setText(label);
}

/* ************************************************************************ */

QString PlotCustomizationDialog::getPlotYLabel() const noexcept
{
    return ui->lineEditY->text();
}

/* ************************************************************************ */

void PlotCustomizationDialog::setPlotYLabel(QString label) noexcept
{
    ui->lineEditY->setText(label);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
