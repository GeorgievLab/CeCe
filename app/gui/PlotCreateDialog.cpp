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

// Qt
#include <QPushButton>

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
    const auto& exporters = m_factory->getExporters();
    for (const auto& exporter : exporters)
    {
        ui->comboBoxSource->addItem(exporter->getName());
    }

    checkValidity();
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

QString PlotCreateDialog::getGroup() const noexcept
{
    return ui->comboBoxGroup->currentText();
}

/* ************************************************************************ */

QString PlotCreateDialog::getType() const noexcept
{
    return ui->comboBoxType->currentText();
}

/* ************************************************************************ */

void PlotCreateDialog::on_comboBoxSource_currentIndexChanged(QString name)
{
    if (name.isEmpty())
    {
        checkValidity();
        return;
    }

    ui->comboBoxAxisX->clear();
    ui->comboBoxAxisY->clear();
    ui->comboBoxGroup->clear();

    // Foreach exporters
    for (const auto& exporter : m_factory->getExporters())
    {
        Q_ASSERT(exporter);

        // Find required exporter
        if (exporter->getName() != name)
            continue;

        // Get value names
        const auto& names = exporter->getNames();

        if (names.empty())
            continue;

        ui->comboBoxAxisX->addItem("");
        ui->comboBoxAxisY->addItem("");
        ui->comboBoxGroup->addItem("");

        for (const auto& column : names)
        {
            ui->comboBoxAxisX->addItem(column);
            ui->comboBoxAxisY->addItem(column);
            ui->comboBoxGroup->addItem(column);
        }
    }

    checkValidity();
}

/* ************************************************************************ */

void PlotCreateDialog::on_comboBoxAxisX_currentIndexChanged(QString name)
{
    checkValidity();
}

/* ************************************************************************ */

void PlotCreateDialog::on_comboBoxAxisY_currentIndexChanged(QString name)
{
    checkValidity();
}

/* ************************************************************************ */

void PlotCreateDialog::on_comboBoxGroup_currentIndexChanged(QString name)
{
    checkValidity();
}

/* ************************************************************************ */

void PlotCreateDialog::checkValidity()
{
    ui->comboBoxAxisX->setEnabled(ui->comboBoxAxisX->count() > 0);
    ui->comboBoxAxisY->setEnabled(ui->comboBoxAxisY->count() > 0);
    ui->comboBoxGroup->setEnabled(ui->comboBoxGroup->count() > 0);

    const bool res =
        ui->comboBoxSource->currentText().isEmpty() ||
        ui->comboBoxAxisX->currentText().isEmpty() ||
        ui->comboBoxAxisY->currentText().isEmpty()
    ;

    // Disable main button
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!res);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
