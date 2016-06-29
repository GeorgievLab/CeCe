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
#include "PlotCreateDialog.hpp"

// UI
#include "ui_PlotCreateDialog.h"

// GUI
#include "DataExportPlot.hpp"
#include "DataExportPlotFactory.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

PlotCreateDialog::PlotCreateDialog(ViewPtr<DataExportPlotFactory> factory, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PlotCreateDialog)
    , m_factory(factory)
{
    ui->setupUi(this);

    // Foreach exporters
    for (const auto& exporter : m_factory->getExporters())
    {
        ui->comboBoxSource->addItem(QString::fromStdString(exporter->getName()));
    }
}

/* ************************************************************************ */

PlotCreateDialog::~PlotCreateDialog()
{
    delete ui;
}

/* ************************************************************************ */

QString PlotCreateDialog::getSource() const noexcept
{
    return ui->comboBoxSource->currentText();
}

/* ************************************************************************ */

QString PlotCreateDialog::getAxisX() const noexcept
{
    return ui->comboBoxAxisX->currentText();
}

/* ************************************************************************ */

QString PlotCreateDialog::getAxisY() const noexcept
{
    return ui->comboBoxAxisY->currentText();
}

/* ************************************************************************ */

QString PlotCreateDialog::getColor() const noexcept
{
    return ui->comboBoxColor->currentText();
}

/* ************************************************************************ */

void PlotCreateDialog::on_comboBoxSource_currentIndexChanged(QString name)
{
    // Foreach exporters
    for (const auto& exporter : m_factory->getExporters())
    {
        Q_ASSERT(exporter);

        // Find required exporter
        if (QString::fromStdString(exporter->getName()) != name)
            continue;

        ui->comboBoxAxisX->addItem("");
        ui->comboBoxAxisY->addItem("");
        ui->comboBoxColor->addItem("");

        for (const auto& column : exporter->getNames())
        {
            QString columnName = QString::fromStdString(column);

            ui->comboBoxAxisX->addItem(columnName);
            ui->comboBoxAxisY->addItem(columnName);
            ui->comboBoxColor->addItem(columnName);
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
