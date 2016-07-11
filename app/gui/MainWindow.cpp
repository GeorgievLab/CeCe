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
#include <QScrollBar>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/DataExport.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/plugin/Context.hpp"
#include "cece/simulator/Visualization.hpp"

// UI
#include "ui_MainWindow.h"

// GUI
#include "AboutDialog.hpp"
#include "PlotCreateDialog.hpp"
#include "PlotWidget.hpp"
#include "DataExportPlot.hpp"
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
    , m_simulator(m_pluginManager.getRepository())
{
    qRegisterMetaType<Log::Type>("Log::Type");

    ui->setupUi(this);

    // Store pointer to simulator in visualization
    ui->visualizationWidget->setSimulator(&m_simulator);

    ui->widgetInitializationInfo->hide();
    ui->widgetModified->hide();
    ui->actionSimulationStart->setEnabled(false);
    ui->actionSimulationStep->setEnabled(false);
    ui->actionSimulationStop->setEnabled(false);
    ui->actionSimulationReset->setEnabled(false);

    ui->menuView->addAction(ui->toolBar->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetVisualization->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetTools->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetLog->toggleViewAction());

    // Set log stream
    Log::setOutput(&m_logStream);
    connect(&m_logStream, &LogStream::append, this, &MainWindow::logAppend);

    // Restore settings
    restoreSettings();

    // Load directories for plugin manager
#ifdef DIR_PLUGINS
    m_pluginManager.addDirectory(DIR_PLUGINS);
#elif __linux__
    m_pluginManager.addDirectory(getPluginsDirectory(argv[0], "../lib/cece/plugins"));
#elif __APPLE__ && __MACH__
    m_pluginManager.addDirectory(getPluginsDirectory(argv[0], "../plugins"));
#elif _WIN32
    m_pluginManager.addDirectory(getPluginsDirectory(argv[0], "."));
#endif

    // Load all plugins
    m_pluginManager.loadAll();

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

    // Set data export factory
    DataExport::setFactory(&m_dataExportFactory);
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

void MainWindow::on_actionFileNew_triggered()
{
    ui->plainTextSourceCode->clear();
    setCurrentFile(QString());
}

/* ************************************************************************ */

void MainWindow::on_actionFileOpen_triggered()
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

void MainWindow::on_actionFileSave_triggered()
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

void MainWindow::on_actionFileSaveAs_triggered()
{
    QString filename =
        QFileDialog::getSaveFileName(this, QString(), m_filename);

    if (filename.isEmpty())
        return;

    fileSave(filename);
}

/* ************************************************************************ */

void MainWindow::on_actionFileRecent1_triggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        fileOpen(action->data().toString());
}

/* ************************************************************************ */

void MainWindow::on_actionFileRecent2_triggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        fileOpen(action->data().toString());
}

/* ************************************************************************ */

void MainWindow::on_actionFileRecent3_triggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        fileOpen(action->data().toString());
}

/* ************************************************************************ */

void MainWindow::on_actionFileRecent4_triggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        fileOpen(action->data().toString());
}

/* ************************************************************************ */

void MainWindow::on_actionFileRecent5_triggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        fileOpen(action->data().toString());
}

/* ************************************************************************ */

void MainWindow::on_actionViewFullscreen_toggled(bool flag)
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

void MainWindow::on_actionSimulationStart_triggered()
{
    Q_ASSERT(!m_simulator.isRunning());
    Q_ASSERT(!m_simulatorThread.isRunning());
    m_simulator.start(Simulator::Mode::Simulate);
}

/* ************************************************************************ */

void MainWindow::on_actionSimulationStop_triggered()
{
    Q_ASSERT(m_simulator.isRunning());
    Q_ASSERT(m_simulatorThread.isRunning());
    m_simulator.stop();
}

/* ************************************************************************ */

void MainWindow::on_actionSimulationStep_triggered()
{
    Q_ASSERT(!m_simulator.isRunning());
    Q_ASSERT(!m_simulatorThread.isRunning());
    m_simulator.step();
}

/* ************************************************************************ */

void MainWindow::on_actionSimulationReset_triggered()
{
    Q_ASSERT(!m_simulator.isRunning());
    Q_ASSERT(!m_simulatorThread.isRunning());
    m_simulator.simulationLoad("cece", ui->plainTextSourceCode->toPlainText(), m_filename);
}

/* ************************************************************************ */

void MainWindow::on_actionVisualizationResetView_triggered()
{
    // Fit simulation to view
    ui->visualizationWidget->fitToView();
}

/* ************************************************************************ */

void MainWindow::on_actionVisualizationScreenshot_triggered()
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

void MainWindow::on_actionPlotCreate_triggered()
{
    PlotCreateDialog dlg(&m_dataExportFactory, this);

    if (dlg.exec() == QDialog::Rejected)
        return;

    QDockWidget* dock = new QDockWidget(this);
    dock->setWindowTitle(dlg.getSource());
    dock->setObjectName(dlg.getSource());
    PlotWidget* widget = new PlotWidget();
    widget->setXAxisName(dlg.getAxisX());
    widget->setYAxisName(dlg.getAxisY());
    widget->setGroupName(dlg.getGroup());
    widget->setType(dlg.getType());

    dock->setWidget(widget);
    dock->setAttribute(Qt::WA_DeleteOnClose, true);
    addDockWidget(static_cast<Qt::DockWidgetArea>(2), dock);

    String source = dlg.getSource().toStdString();
    auto exporter = m_dataExportFactory.getExporter(source);

    connect(exporter.get(), &DataExportPlot::dataAdded, widget, &PlotWidget::dataAdd);

    // Move exporter to simulation thread
    exporter->moveToThread(&m_simulatorThread);
}

/* ************************************************************************ */

void MainWindow::on_actionHelpAbout_triggered()
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

    ui->actionSimulationStart->setEnabled(flag);
    ui->actionSimulationStep->setEnabled(flag);
    ui->actionSimulationReset->setEnabled(flag);
    ui->widgetModified->hide();
    ui->progressBar->setValue(0);

    // Fit simulation to view
    ui->visualizationWidget->fitToView();

    // Clear visualization layer actions
    for (auto action : m_visualizationActions)
        delete action;

    m_visualizationActions.clear();
    m_dataExportFactory.reset();

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

    // Initialize simulation by default
    emit m_simulator.start(Simulator::Mode::Initialize);
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
        ui->actionSimulationStart->setEnabled(false);
        ui->actionSimulationStop->setEnabled(false);
        ui->actionSimulationStep->setEnabled(false);
        ui->actionSimulationReset->setEnabled(false);
        break;

    case Simulator::Mode::Simulate:
        ui->actionSimulationStart->setEnabled(false);
        ui->actionSimulationStop->setEnabled(true);
        ui->actionSimulationStep->setEnabled(false);
        ui->actionSimulationReset->setEnabled(false);
        break;
    }
}

/* ************************************************************************ */

void MainWindow::simulatorStepped(Simulator::Mode mode, int iteration)
{
    ui->progressBar->setValue(iteration);
}

/* ************************************************************************ */

void MainWindow::simulatorFinished(Simulator::Mode mode, bool end)
{
    switch (mode)
    {
    case Simulator::Mode::Idle:
        // Ignore
        break;

    case Simulator::Mode::Initialize:
        ui->widgetInitializationInfo->hide();

        if (m_simulator.getSimulation() && m_simulator.getSimulation()->isInitialized())
        {
            ui->actionSimulationStart->setEnabled(true);
            ui->actionSimulationStop->setEnabled(false);
            ui->actionSimulationStep->setEnabled(true);
            ui->actionSimulationReset->setEnabled(true);
        }
        else
        {
            ui->actionSimulationStart->setEnabled(false);
            ui->actionSimulationStop->setEnabled(false);
            ui->actionSimulationStep->setEnabled(false);
            ui->actionSimulationReset->setEnabled(true);
        }
        break;

    case Simulator::Mode::Simulate:
        ui->actionSimulationStart->setEnabled(true);
        ui->actionSimulationStop->setEnabled(false);
        ui->actionSimulationStep->setEnabled(!end);
        ui->actionSimulationReset->setEnabled(true);
        break;
    }
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

void MainWindow::logAppend(Log::Type type, QString section, QString message)
{
    QString msg;

    switch (type)
    {
    case Log::Type::Default: break;
    case Log::Type::Info:    msg += "<font style='color: blue'>[INFO]</font> "; break;
    case Log::Type::Warning: msg += "<font style='color: yellow'>[WARN]</font> "; break;
    case Log::Type::Error:   msg += "<font style='color: red'>[ERROR]</font> "; break;
    case Log::Type::Debug:   msg += "<font style='color: green'>[DEBUG]</font> "; break;
    }

    if (!section.isEmpty())
        msg += "[" + section + "] ";

    msg += message;

    ui->textEditLog->append(msg);

    QScrollBar* sb = ui->textEditLog->verticalScrollBar();
    sb->setValue(sb->maximum());
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
    m_recentFiles[0] = ui->actionFileRecent1;
    m_recentFiles[1] = ui->actionFileRecent2;
    m_recentFiles[2] = ui->actionFileRecent3;
    m_recentFiles[3] = ui->actionFileRecent4;
    m_recentFiles[4] = ui->actionFileRecent5;

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
    const plugin::Repository& repository = m_pluginManager.getRepository();

    // Foreach repository records
    for (const auto& p : repository.getRecords())
    {
        const String pluginName = String(m_pluginManager.getName(p.first));
        const plugin::RepositoryRecord& record = p.second;

        // Foreach initializers
        for (const auto& name : record.getInitFactoryManager().getNames())
        {
            ui->treeWidgetTools->addTopLevelItem(
                new QTreeWidgetItem(ui->treeWidgetTools, QStringList{
                    QString::fromStdString(pluginName),
                    "init",
                    QString::fromStdString(name)
                })
            );
        }

        // Foreach modules
        for (const auto& name : record.getModuleFactoryManager().getNames())
        {
            ui->treeWidgetTools->addTopLevelItem(
                new QTreeWidgetItem(ui->treeWidgetTools, QStringList{
                    QString::fromStdString(pluginName),
                    "module",
                    QString::fromStdString(name)
                })
            );
        }

        // Foreach objects
        for (const auto& name : record.getObjectFactoryManager().getNames())
        {
            ui->treeWidgetTools->addTopLevelItem(
                new QTreeWidgetItem(ui->treeWidgetTools, QStringList{
                    QString::fromStdString(pluginName),
                    "object",
                    QString::fromStdString(name)
                })
            );
        }

        // Foreach programs
        for (const auto& name : record.getProgramFactoryManager().getNames())
        {
            ui->treeWidgetTools->addTopLevelItem(
                new QTreeWidgetItem(ui->treeWidgetTools, QStringList{
                    QString::fromStdString(pluginName),
                    "program",
                    QString::fromStdString(name)
                })
            );
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
