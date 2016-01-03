/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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
#include "MainWindow.hpp"

// Qt
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>

// UI
#include "ui_MainWindow.h"

// CeCe
#include "AboutDialog.hpp"

/* ************************************************************************ */

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionReset->setEnabled(false);
}

/* ************************************************************************ */

MainWindow::~MainWindow()
{
    delete ui;
}

/* ************************************************************************ */

void MainWindow::fileNew()
{
    ui->plainTextEdit->clear();
    setCurrentFile(QString());
}

/* ************************************************************************ */

void MainWindow::fileOpen()
{
    QString filename = QFileDialog::getOpenFileName(this);

    if (filename.isEmpty())
        return;

    fileOpen(filename);
}

/* ************************************************************************ */

void MainWindow::fileSave()
{
    QString filename = m_filename;

    if (!QFileInfo(filename).exists())
    {
        filename = QFileDialog::getSaveFileName(this, QString(), filename);

        if (filename.isEmpty())
            return;
    }

    fileSave(filename);
}

/* ************************************************************************ */

void MainWindow::fileSaveAs()
{
    QString filename =
        QFileDialog::getSaveFileName(this, QString(), m_filename);

    if (filename.isEmpty())
        return;

    fileSave(filename);
}

/* ************************************************************************ */

void MainWindow::viewToolbar(bool flag)
{
    ui->toolBar->setVisible(flag);
}

/* ************************************************************************ */

void MainWindow::viewFullscreen(bool flag)
{
    if (flag)
    {
        m_previousState = windowState();
        showFullScreen();
    }
    else
    {
        setWindowState(m_previousState);
    }
}

/* ************************************************************************ */

void MainWindow::simulationStart()
{
}

/* ************************************************************************ */

void MainWindow::simulationPause()
{
}

/* ************************************************************************ */

void MainWindow::simulationStep()
{
}

/* ************************************************************************ */

void MainWindow::simulationReset()
{
}

/* ************************************************************************ */

void MainWindow::helpAbout()
{
    AboutDialog(this).exec();
}

/* ************************************************************************ */

void MainWindow::setCurrentFile(QString filename)
{
    m_filename = filename;
    setWindowFilePath(m_filename);
}

/* ************************************************************************ */

void MainWindow::fileOpen(QString filename)
{
    QFile file(filename);

    // Open for reading
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, tr("Error"), file.errorString());
        return;
    }

    QTextStream in(&file);
    ui->plainTextEdit->setPlainText(in.readAll());
    setCurrentFile(filename);
}

/* ************************************************************************ */

void MainWindow::fileSave(QString filename)
{
    QFile file(filename);

    // Open for reading
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(this, tr("Error"), file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui->plainTextEdit->toPlainText();
    setCurrentFile(filename);
}

/* ************************************************************************ */
