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
#include <QSettings>
#include <QStringList>

// CeCe
#include "cece/plugin/Manager.hpp"

// UI
#include "ui_MainWindow.h"

// GUI
#include "AboutDialog.hpp"

/* ************************************************************************ */

constexpr int MainWindow::MAX_RECENT_FILES;

/* ************************************************************************ */

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionStart->setEnabled(false);
    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionReset->setEnabled(false);

    restoreSettings();
    initRecentFiles();

    m_pluginsItem = new QTreeWidgetItem(ui->treeWidget);
    m_pluginsItem->setText(0, tr("Plugins"));

    // Foreach available plugins
    for (const auto& plugin : cece::plugin::Manager::s().getNames())
    {
        auto pluginItem = new QTreeWidgetItem(m_pluginsItem);
        pluginItem->setText(0, QString::fromStdString(plugin));
    }

    initSimulator();
}

/* ************************************************************************ */

void MainWindow::closeEvent(QCloseEvent* event)
{
    storeSettings();
    event->accept();
}

/* ************************************************************************ */

MainWindow::~MainWindow()
{
    m_simulatorThread.quit();
    m_simulatorThread.wait();
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

void MainWindow::fileRecentOpen()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        fileOpen(action->data().toString());
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
    emit simulatorStart();
}

/* ************************************************************************ */

void MainWindow::simulationPause()
{
    emit simulatorPause();
}

/* ************************************************************************ */

void MainWindow::simulationStep()
{
    emit simulatorStep();
}

/* ************************************************************************ */

void MainWindow::simulationReset()
{
    emit simulatorReset();
}

/* ************************************************************************ */

void MainWindow::helpAbout()
{
    AboutDialog(this).exec();
}

/* ************************************************************************ */

void MainWindow::simulatorRunning(bool flag)
{
    ui->actionStart->setEnabled(!flag);
    ui->actionStep->setEnabled(!flag);
    ui->actionPause->setEnabled(flag);
}

/* ************************************************************************ */

void MainWindow::simulatorLoaded(bool flag)
{
    ui->actionStart->setEnabled(flag);
    ui->actionStep->setEnabled(flag);
}

/* ************************************************************************ */

void MainWindow::simulatorError(QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}

/* ************************************************************************ */

void MainWindow::simulatorStepped(int iteration, int iterations)
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(iterations);
    ui->progressBar->setValue(iteration);
}

/* ************************************************************************ */

void MainWindow::editTreeItemSelected(QTreeWidgetItem* item, int)
{
    if (item->parent() == m_pluginsItem)
    {
        ui->plainTextEdit->insertPlainText(
            QString("<plugin name=\"%1\" />\n").arg(item->text(0)));
    }
}

/* ************************************************************************ */

void MainWindow::setCurrentFile(QString filename)
{
    m_filename = filename;
    setWindowFilePath(m_filename);

    // Get stored list of recent files
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(m_filename);
    files.prepend(m_filename);

    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();

    // Store updated list
    settings.setValue("recentFileList", files);

    updateRecentFileActions();
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

    emit simulatorSource(ui->plainTextEdit->toPlainText(), "cece");
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

    emit simulatorSource(ui->plainTextEdit->toPlainText(), "cece");
}

/* ************************************************************************ */

void MainWindow::storeSettings() const
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

/* ************************************************************************ */

void MainWindow::restoreSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

/* ************************************************************************ */

void MainWindow::initRecentFiles()
{
    m_recentFiles[0] = ui->actionRecentFile1;
    m_recentFiles[1] = ui->actionRecentFile2;
    m_recentFiles[2] = ui->actionRecentFile3;
    m_recentFiles[3] = ui->actionRecentFile4;
    m_recentFiles[4] = ui->actionRecentFile5;

    for (int i = 0; i < MAX_RECENT_FILES; ++i)
        m_recentFiles[i]->setVisible(false);

    updateRecentFileActions();
}

/* ************************************************************************ */

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), MAX_RECENT_FILES);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text =
            tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        m_recentFiles[i]->setText(text);
        m_recentFiles[i]->setData(files[i]);
        m_recentFiles[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MAX_RECENT_FILES; ++j)
        m_recentFiles[j]->setVisible(false);
}

/* ************************************************************************ */

void MainWindow::initSimulator()
{
    // Connect thread events
    connect(this, &MainWindow::simulatorStart, &m_simulator, &Simulator::start);
    connect(this, &MainWindow::simulatorStep, &m_simulator, &Simulator::step);
    connect(this, &MainWindow::simulatorPause, &m_simulator, &Simulator::pause);
    connect(this, &MainWindow::simulatorReset, &m_simulator, &Simulator::reset);
    connect(this, &MainWindow::simulatorSource, &m_simulator,
        &Simulator::createSimulation);
    connect(
        &m_simulator, &Simulator::loaded, this, &MainWindow::simulatorLoaded);
    connect(
        &m_simulator, &Simulator::loadError, this, &MainWindow::simulatorError);
    connect(
        &m_simulator, &Simulator::running, this, &MainWindow::simulatorRunning);
    connect(
        &m_simulator, &Simulator::stepped, this, &MainWindow::simulatorStepped);

    // Stop simulation
    connect(&m_simulatorThread, &QThread::finished, &m_simulator,
        &Simulator::pause);

    // Move simulator into the simulator thread
    m_simulator.moveToThread(&m_simulatorThread);

    // Start the simulator thread
    m_simulatorThread.start();
}

/* ************************************************************************ */
