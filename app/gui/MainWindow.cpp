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
#include <QImage>
#include <QFileInfo>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/plugin/Context.hpp"

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
    , m_simulator(plugin::Manager::s())
{
    ui->setupUi(this);

    // Store pointer to simulator in visualization
    ui->visualizationWidget->setSimulator(&m_simulator);

    ui->widgetInitializationInfo->hide();
    ui->widgetModified->hide();
    ui->actionStart->setEnabled(false);
    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionReset->setEnabled(false);

    ui->menuView->addAction(ui->toolBar->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetVisualization->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetTools->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetLog->toggleViewAction());

    // Set log stream
    Log::setOutput(&m_logStream);
    connect(&m_logStream, &LogStream::append, ui->textEditLog, &QTextEdit::append);

    // Restore settings
    restoreSettings();

    new XmlHighlighter(ui->plainTextSourceCode->document());

    // Move simulator into the simulator thread
    m_simulator.moveToThread(&m_simulatorThread);

    connect(&m_simulator, &Simulator::startRequest, [this] () {
        m_simulatorThread.start();
    });
    connect(&m_simulatorThread, &QThread::started, &m_simulator, &Simulator::run);
    connect(&m_simulator, &Simulator::finished, &m_simulatorThread, &QThread::quit, Qt::DirectConnection);

    // Connect thread events
    connect(&m_simulator, &Simulator::simulationLoaded, this, &MainWindow::simulatorLoaded);
    connect(&m_simulator, &Simulator::error, this, &MainWindow::simulatorError);
    connect(&m_simulator, &Simulator::started, this, &MainWindow::simulatorStarted);
    connect(&m_simulator, &Simulator::stepped, this, &MainWindow::simulatorStepped);
    connect(&m_simulator, &Simulator::finished, this, &MainWindow::simulatorFinished);

    // Simulation drawer
    connect(&m_simulatorDrawTimer, &QTimer::timeout, [this]() {
        ui->visualizationWidget->update();
    });

    // Start draw timer
    m_simulatorDrawTimer.start(1000 / 30);

    // Initialize tools from simulator
    initTools();
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
    m_simulator.stop();
    m_simulatorThread.quit();
    m_simulatorThread.wait();
    delete ui;

    // Disable output
    Log::setOutput(nullptr);
}

/* ************************************************************************ */

void MainWindow::fileNew()
{
    ui->plainTextSourceCode->clear();
    setCurrentFile(QString());
}

/* ************************************************************************ */

void MainWindow::fileOpen()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString(), QString(),
        tr("CeCe file (*.cece)")
    );

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
        filename = QFileDialog::getSaveFileName(this,
            QString(), filename,
            tr("CeCe file (*.cece)")
        );

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
    m_simulator.start(Simulator::Mode::Simulate);
}

/* ************************************************************************ */

void MainWindow::simulationStop()
{
    Q_ASSERT(m_simulator.isRunning());
    Q_ASSERT(m_simulatorThread.isRunning());
    m_simulator.stop();
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
    m_simulator.simulationLoad("cece", ui->plainTextSourceCode->toPlainText(), m_filename);
}

/* ************************************************************************ */

void MainWindow::visualizationScreenshot()
{
    const QString base = !m_filename.isEmpty() ? QFileInfo(m_filename).completeBaseName() : "cece";

    QString filename = QFileDialog::getSaveFileName(this,
        QString(), QString("%1-%2.%3").arg(base, QString::number(ui->progressBar->value()), "png"),
        tr("Images (*.png *.jpg)")
    );

    if (filename.isEmpty())
        return;

    QImage image = ui->visualizationWidget->grabFrameBuffer();
    image.save(filename);
}

/* ************************************************************************ */

void MainWindow::helpAbout()
{
    AboutDialog(this).exec();
}

/* ************************************************************************ */

void MainWindow::sourceCodeWasModified(bool flag)
{
    setWindowModified(flag);

    ui->widgetModified->show();
}

/* ************************************************************************ */

void MainWindow::simulatorLoaded(simulator::Simulation* simulation)
{
    const bool flag = simulation != nullptr;

    ui->actionStart->setEnabled(flag);
    ui->actionStep->setEnabled(flag);
    ui->actionReset->setEnabled(flag);
    ui->widgetModified->hide();

    // Clear visualization layer actions
    for (auto action : m_visualizationActions)
        delete action;

    m_visualizationActions.clear();

    if (flag)
    {
        // Store original format
        static auto format = ui->progressBar->format();

        ui->progressBar->setMinimum(0);

        if (simulation->hasUnlimitedIterations())
        {
            ui->progressBar->setMaximum(std::numeric_limits<int>::max());
            ui->progressBar->setFormat("%v");
        }
        else
        {
            ui->progressBar->setMaximum(simulation->getIterations());
            ui->progressBar->setFormat(format);
        }

        // Add visualization layer actions
        for (const auto& layer : simulation->getVisualization().getLayers())
        {
            auto action = new QAction(QString::fromStdString(layer.getName()), this);
            action->setCheckable(true);
            action->setChecked(layer.isEnabled());
            m_visualizationActions.append(action);
            ui->menuVisualization->addAction(action);

            connect(action, &QAction::toggled, this, &MainWindow::visualizationLayerToggle);
        }
    }
}

/* ************************************************************************ */

void MainWindow::simulatorError(Simulator::Mode mode, QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}

/* ************************************************************************ */

void MainWindow::simulatorStarted(Simulator::Mode mode)
{
    switch (mode)
    {
    case Simulator::Mode::Idle:
        // Ignore
        break;

    case Simulator::Mode::Initialize:
        ui->widgetInitializationInfo->show();
        ui->actionStart->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->actionStep->setEnabled(false);
        ui->actionReset->setEnabled(false);
        break;

    case Simulator::Mode::Simulate:
        ui->actionStart->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->actionStep->setEnabled(false);
        ui->actionReset->setEnabled(false);
        break;
    }
}

/* ************************************************************************ */

void MainWindow::simulatorStepped(Simulator::Mode mode, int iteration)
{
    ui->progressBar->setValue(iteration);
}

/* ************************************************************************ */

void MainWindow::simulatorFinished(Simulator::Mode mode)
{
    switch (mode)
    {
    case Simulator::Mode::Idle:
        // Ignore
        break;

    case Simulator::Mode::Initialize:
        ui->widgetInitializationInfo->hide();
        ui->actionStart->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStep->setEnabled(true);
        ui->actionReset->setEnabled(true);
        break;

    case Simulator::Mode::Simulate:
        ui->actionStart->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStep->setEnabled(false);
        ui->actionReset->setEnabled(true);
        break;
    }
}

/* ************************************************************************ */

void MainWindow::simulatorInitializationStart()
{
    m_simulator.start(Simulator::Mode::Initialize);
}

/* ************************************************************************ */

void MainWindow::simulatorInitializationCancel()
{
    if (m_simulator.getMode() == Simulator::Mode::Initialize)
        m_simulator.stop();
}

/* ************************************************************************ */

void MainWindow::visualizationLayerToggle(bool flag)
{
    QAction* action = qobject_cast<QAction*>(sender());

    if (!action)
        return;

    // Get visualization
    auto& visualization = m_simulator.getSimulation()->getVisualization();

    for (auto& layer : visualization.getLayers())
    {
        if (layer.getName() == action->text().toStdString())
        {
            layer.setEnabled(flag);
            break;
        }
    }
}

/* ************************************************************************ */

void MainWindow::setCurrentFile(QString filename)
{
    m_filename = filename;
    //setWindowFilePath(m_filename);

    if (!m_filename.isEmpty())
    {
        QFileInfo info(m_filename);
        setWindowTitle(QString("[*] %1 (~ %2)").arg(info.fileName(), info.path()));
    }
    else
    {
        setWindowTitle("[*]");
    }

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
    ui->plainTextSourceCode->setPlainText(in.readAll());
    setCurrentFile(filename);

    // Stop previous simulation
    m_simulator.stop();
    m_simulatorThread.quit();
    m_simulatorThread.wait();
    m_simulator.simulationLoad("cece", ui->plainTextSourceCode->toPlainText(), m_filename);
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
    out << ui->plainTextSourceCode->toPlainText();
    setCurrentFile(filename);
    ui->plainTextSourceCode->document()->setModified(false);
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

    // Restore recent files
    initRecentFiles();
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
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        m_recentFiles[i]->setText(text);
        m_recentFiles[i]->setData(files[i]);
        m_recentFiles[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MAX_RECENT_FILES; ++j)
        m_recentFiles[j]->setVisible(false);
}

/* ************************************************************************ */

void MainWindow::initTools()
{
    const plugin::Manager& manager = plugin::Manager::s();
    const plugin::Context& context = manager.getContext();

    // Foreach initializers
    for (const auto& name : context.getInitFactoryManager().getNames())
    {
        ui->treeWidgetTools->addTopLevelItem(
            new QTreeWidgetItem(ui->treeWidgetTools, QStringList{"init", QString::fromStdString(name)})
        );
    }

    // Foreach modules
    for (const auto& name : context.getModuleFactoryManager().getNames())
    {
        ui->treeWidgetTools->addTopLevelItem(
            new QTreeWidgetItem(ui->treeWidgetTools, QStringList{"module", QString::fromStdString(name)})
        );
    }

    // Foreach objects
    for (const auto& name : context.getObjectFactoryManager().getNames())
    {
        ui->treeWidgetTools->addTopLevelItem(
            new QTreeWidgetItem(ui->treeWidgetTools, QStringList{"object", QString::fromStdString(name)})
        );
    }

    // Foreach programs
    for (const auto& name : context.getProgramFactoryManager().getNames())
    {
        ui->treeWidgetTools->addTopLevelItem(
            new QTreeWidgetItem(ui->treeWidgetTools, QStringList{"program", QString::fromStdString(name)})
        );
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
