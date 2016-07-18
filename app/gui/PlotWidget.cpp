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

// Qt
#include <QFileDialog>
#include <QPixmap>
#include <QFile>
#include <QTextStream>

// Ui
#include "ui_PlotWidget.h"

// GUI
#include "PlotCustomizationDialog.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlotWidget)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);

    ui->plot->addAction(ui->actionCustomize);
    ui->plot->addAction(ui->actionSave);
    ui->plot->addAction(ui->actionSaveData);
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

        // Show legend
        if (i > 1)
            ui->plot->legend->setVisible(true);
    }

    graph->addData(xValue, yValue);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

/* ************************************************************************ */

void PlotWidget::on_actionCustomize_triggered()
{
    PlotCustomizationDialog dlg(this);

    // Get layout
    QCPLayoutGrid* layout = ui->plot->plotLayout();
    QCPPlotTitle* title = nullptr;

    // Find title element
    for (int i = 0; i < layout->elementCount(); ++i)
    {
         if ((title = qobject_cast<QCPPlotTitle*>(layout->elementAt(i))))
             break;
    }

    dlg.setPlotTitle(title ? title->text() : QString());
    dlg.setPlotXLabel(ui->plot->xAxis->label());
    dlg.setPlotYLabel(ui->plot->yAxis->label());

    if (dlg.exec() == QDialog::Rejected)
        return;

    if (!dlg.getPlotTitle().isEmpty())
    {
        if (title)
        {
            title->setText(dlg.getPlotTitle());
        }
        else
        {
            layout->insertRow(0);
            layout->addElement(0, 0, new QCPPlotTitle(ui->plot, dlg.getPlotTitle()));
        }
    }
    else if (title)
    {
        layout->remove(title);
    }

    ui->plot->xAxis->setLabel(dlg.getPlotXLabel());
    ui->plot->yAxis->setLabel(dlg.getPlotYLabel());

    ui->plot->replot();
    update();
}

/* ************************************************************************ */

void PlotWidget::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        QString(), QStringLiteral("plot.png"),
        tr("Images (*.png *.jpg)")
    );

    if (filename.isEmpty())
        return;

    // Plot pixels
    QPixmap pixmap = ui->plot->toPixmap();

    // Save pixmap
    pixmap.save(filename);
}

/* ************************************************************************ */

void PlotWidget::on_actionSaveData_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        QString(), QStringLiteral("plot.csv"),
        tr("CSV file (*.csv)")
    );

    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return;

    QTextStream output(&file);

    // Write header
    output << m_groupName << ";" << m_xName << ";" << m_yName << "\n";

    for (int i = 0; i < ui->plot->graphCount(); ++i)
    {
        QCPGraph* graph = ui->plot->graph(i);
        Q_ASSERT(graph);
        QCPDataMap* data = graph->data();
        Q_ASSERT(data);

        for (const auto& p : *data)
        {
            // Write row
            output << graph->name() << ";" << p.key << ";" << p.value << "\n";
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
