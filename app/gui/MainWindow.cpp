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

    connect(&m_simulator, SIGNAL(simulationStarted()), &m_simulatorThread, SLOT(start()));
    connect(&m_simulatorThread, &QThread::started, &m_simulator, &Simulator::simulate);
    connect(&m_simulator, &Simulator::simulationFinished, &m_simulatorThread, &QThread::quit, Qt::DirectConnection);
    connect(&m_simulator, &Simulator::simulationPaused, &m_simulatorThread, &QThread::quit, Qt::DirectConnection);

    // Connect thread events
    connect(&m_simulator, &Simulator::simulationLoaded, this, &MainWindow::simulatorLoaded);
    connect(&m_simulator, &Simulator::simulationError, this, &MainWindow::simulatorError);
    connect(&m_simulator, &Simulator::simulationStarted, this, &MainWindow::simulatorStarted);
    connect(&m_simulator, &Simulator::simulationStepped, this, &MainWindow::simulatorStepped);
    connect(&m_simulator, &Simulator::simulationPaused, this, &MainWindow::simulatorPaused);
    connect(&m_simulator, &Simulator::simulationFinished, this, &MainWindow::simulatorFinished);

    // Simulation drawer
    connect(&m_simulatorDrawTimer, &QTimer::timeout, [this]() {
        ui->visualizationWidget->update();
    });

    // Start draw timer
    m_simulatorDrawTimer.start(1000 / 30);
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
    m_simulator.simulationLoad("cece", ui->plainTextSourceCode->toPlainText());
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
}

/* ************************************************************************ */

void MainWindow::simulatorLoaded(simulator::Simulation* simulation)
{
    const bool flag = simulation != nullptr;

    ui->actionStart->setEnabled(flag);
    ui->actionStep->setEnabled(flag);
    ui->actionReset->setEnabled(flag);

    if (flag)
    {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(simulation->getIterations());
    }
}

/* ************************************************************************ */

void MainWindow::simulatorError(QString message)
{
    QMessageBox::critical(this, tr("Error"), message);
}

/* ************************************************************************ */

void MainWindow::simulatorStarted()
{
    ui->actionStart->setEnabled(false);
    ui->actionPause->setEnabled(true);
    ui->actionStep->setEnabled(false);
    ui->actionReset->setEnabled(false);
}

/* ************************************************************************ */

void MainWindow::simulatorPaused()
{
    ui->actionStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionStep->setEnabled(true);
    ui->actionReset->setEnabled(true);
}

/* ************************************************************************ */

void MainWindow::simulatorStepped(int iteration)
{
    ui->progressBar->setValue(iteration);
}

/* ************************************************************************ */

void MainWindow::simulatorFinished()
{
    ui->actionStart->setEnabled(false);
    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionReset->setEnabled(true);
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

    m_simulator.simulationLoad("cece", ui->plainTextSourceCode->toPlainText());
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

}
}

/* ************************************************************************ */
