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

// C++
#include <iostream>

// Qt
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QSettings>
#include <QStringList>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/plugin/Manager.hpp"

// UI
#include "ui_MainWindow.h"

// GUI
#include "AboutDialog.hpp"
#include "XmlHighlighter.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

constexpr int MainWindow::MAX_RECENT_FILES;

/* ************************************************************************ */

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->reloadButton->hide();

    new XmlHighlighter(ui->plainTextEdit->document());

    // Set log stream
    Log::setOutput(&m_logStream);
    connect(&m_logStream, &LogStream::append, ui->textEditLog, &QTextEdit::append);

    ui->actionStart->setEnabled(false);
    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionReset->setEnabled(false);

    restoreSettings();
    initRecentFiles();
    initPlugins();
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
    m_simulator.pause();
    m_simulatorThread.quit();
    m_simulatorThread.wait();
    delete ui;

    // Disable output
    Log::setOutput(nullptr);
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
    Q_ASSERT(!m_simulator.isRunning());
    Q_ASSERT(!m_simulatorThread.isRunning());
    m_simulator.start();
}

/* ************************************************************************ */

void MainWindow::simulationPause()
{
    Q_ASSERT(m_simulator.isRunning());
    Q_ASSERT(m_simulatorThread.isRunning());
    m_simulator.pause();
}

/* ************************************************************************ */

void MainWindow::simulationStep()
{
    Q_ASSERT(!m_simulator.isRunning());
    Q_ASSERT(!m_simulatorThread.isRunning());
    m_simulator.step();
}

/* ************************************************************************ */

void MainWindow::simulationReset()
{
    Q_ASSERT(!m_simulator.isRunning());
    Q_ASSERT(!m_simulatorThread.isRunning());
    m_simulator.reset();
    ui->reloadButton->hide();
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
    ui->actionReset->setEnabled(flag);
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

void MainWindow::simulatorFinished(bool end)
{
    ui->actionStart->setEnabled(!end);
    ui->actionStep->setEnabled(!end);
    ui->actionPause->setEnabled(end);
    ui->actionReset->setEnabled(true);
}

/* ************************************************************************ */

void MainWindow::editTreeItemSelected(QTreeWidgetItem* item, int)
{
    if (item->parent() == m_pluginsItem)
    {
        ui->plainTextEdit->insertPlainText(QString(
            "<plugin name=\"%1\" />\n"
        ).arg(item->text(0)));
    }
    else if (item->parent() == m_modulesItem)
    {
        ui->plainTextEdit->insertPlainText(QString(
            "<module name=\"%1\" />\n"
        ).arg(item->text(0)));
    }
    else if (item->parent() == m_objectsItem)
    {
        ui->plainTextEdit->insertPlainText(QString(
            "<object class=\"%1\" />\n"
        ).arg(item->text(0)));
    }
    else if (item->parent() == m_programsItem)
    {
        ui->plainTextEdit->insertPlainText(QString(
            "<program language=\"%1\">\n"
            "<![CDATA[\n"
            "\n"
            "]]>\n"
            "</program>\n"
        ).arg(item->text(0)));
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

    if (!m_filename.isEmpty())
    {
        files.removeAll(m_filename);
        files.prepend(m_filename);
    }

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

    m_simulator.createSimulation(ui->plainTextEdit->toPlainText(), "cece");
    ui->reloadButton->hide();
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

    m_simulator.createSimulation(ui->plainTextEdit->toPlainText(), "cece");
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
    // Set simulator
    ui->openGLWidget->setSimulator(&m_simulator);

    // Move simulator into the simulator thread
    m_simulator.moveToThread(&m_simulatorThread);

    connect(&m_simulator, SIGNAL(simulationStarted()), &m_simulatorThread, SLOT(start()));
    connect(&m_simulatorThread, &QThread::started, &m_simulator, &Simulator::simulate);
    connect(&m_simulator, &Simulator::simulationFinished, &m_simulatorThread, &QThread::quit, Qt::DirectConnection);

    // Connect thread events
    connect(&m_simulator, &Simulator::loaded, this, &MainWindow::simulatorLoaded);
    connect(&m_simulator, &Simulator::loadError, this, &MainWindow::simulatorError);
    connect(&m_simulator, &Simulator::running, this, &MainWindow::simulatorRunning);
    connect(&m_simulator, &Simulator::stepped, this, &MainWindow::simulatorStepped);
    connect(&m_simulator, &Simulator::simulationFinished, this, &MainWindow::simulatorFinished);
    connect(&m_simulator, &Simulator::simulationError, this, &MainWindow::simulatorError);

    connect(&m_simulatorDrawTimer, &QTimer::timeout, [this]() {
        ui->openGLWidget->update();
    });

    // Start draw timer
    m_simulatorDrawTimer.start(1000 / 30);
}

/* ************************************************************************ */

void MainWindow::initPlugins()
{
    auto& manager = cece::plugin::Manager::s();
    auto& context = manager.getContext();

    manager.loadAll();

    m_pluginsItem = new QTreeWidgetItem(ui->treeWidget);
    m_pluginsItem->setText(0, tr("Plugins"));

    // Foreach available plugins
    for (const auto& plugin : manager.getNames())
    {
        auto item = new QTreeWidgetItem(m_pluginsItem);
        item->setText(0, QString::fromStdString(plugin));
    }

    m_modulesItem = new QTreeWidgetItem(ui->treeWidget);
    m_modulesItem->setText(0, tr("Modules"));

    // Foreach available modules
    for (const auto& module : context.getModuleFactoryManager().getNames())
    {
        auto item = new QTreeWidgetItem(m_modulesItem);
        item->setText(0, QString::fromStdString(module));
    }

    m_objectsItem = new QTreeWidgetItem(ui->treeWidget);
    m_objectsItem->setText(0, tr("Objects"));

    // Foreach available objects
    for (const auto& object : context.getObjectFactoryManager().getNames())
    {
        auto item = new QTreeWidgetItem(m_objectsItem);
        item->setText(0, QString::fromStdString(object));
    }

    m_programsItem = new QTreeWidgetItem(ui->treeWidget);
    m_programsItem->setText(0, tr("Programs"));

    // Foreach available programs
    for (const auto& program : context.getProgramFactoryManager().getNames())
    {
        auto item = new QTreeWidgetItem(m_programsItem);
        item->setText(0, QString::fromStdString(program));
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
