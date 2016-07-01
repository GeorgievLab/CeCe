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

#pragma once

/* ************************************************************************ */

// Qt
#include <QMainWindow>
#include <QThread>
#include <QTreeWidget>
#include <QTimer>
#include <QList>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/plugin/Manager.hpp"

// GUI
#include "LogStream.hpp"
#include "Simulator.hpp"
#include "DataExportPlotFactory.hpp"

/* ************************************************************************ */

namespace Ui { class MainWindow; }

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief Main GUI window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT


// Public Constants
public:


    /**
     * @brief Maximum recent files.
     */
    static constexpr int MAX_RECENT_FILES = 5;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     * @param parent
     */
    explicit MainWindow(QWidget* parent = nullptr);


    /**
     * @brief Destructor.
     */
    ~MainWindow();


// Public Events
public:


    /**
     * @brief Close event.
     * @param event
     */
    void closeEvent(QCloseEvent* event);


public slots:


    /**
     * @brief Create a new simulation file.
     */
    void on_actionFileNew_triggered();


    /**
     * @brief Open an existing simulation file.
     */
    void on_actionFileOpen_triggered();


    /**
     * @brief Save current simulation file.
     */
    void on_actionFileSave_triggered();


    /**
     * @brief Save current simulation file as.
     */
    void on_actionFileSaveAs_triggered();


    /**
     * @brief Open recent file.
     */
    void on_actionFileRecent1_triggered();


    /**
     * @brief Open recent file.
     */
    void on_actionFileRecent2_triggered();


    /**
     * @brief Open recent file.
     */
    void on_actionFileRecent3_triggered();


    /**
     * @brief Open recent file.
     */
    void on_actionFileRecent4_triggered();


    /**
     * @brief Open recent file.
     */
    void on_actionFileRecent5_triggered();


    /**
     * @brief Toggle fullscreen mode.
     */
    void on_actionViewFullscreen_toggled(bool);


    /**
     * @brief Start simulation.
     */
    void on_actionSimulationStart_triggered();


    /**
     * @brief Stop simulation.
     */
    void on_actionSimulationStop_triggered();


    /**
     * @brief Step simulation.
     */
    void on_actionSimulationStep_triggered();


    /**
     * @brief Reset simulace.
     */
    void on_actionSimulationReset_triggered();


    /**
     * @brief Reset view for visualization.
     */
    void on_actionVisualizationResetView_triggered();


    /**
     * @brief Create a screenshot from visualization.
     */
    void on_actionVisualizationScreenshot_triggered();


    /**
     * @brief On plot create requirest.
     */
    void on_actionPlotCreate_triggered();


    /**
     * @brief Show about dialog.
     */
    void on_actionHelpAbout_triggered();


    /**
     * @brief Source code has been modified.
     */
    void sourceCodeWasModified(bool flag);


    /**
     * @brief Simulator loads new simulation.
     *
     * @param simulation
     */
    void simulatorLoaded(simulator::Simulation* simulation);


    /**
     * @brief On simulator error.
     *
     * @param mode
     * @param message
     */
    void simulatorError(Simulator::Mode mode, QString message);


    /**
     * @brief On simulator start.
     *
     * @param mode
     */
    void simulatorStarted(Simulator::Mode mode);


    /**
     * @brief A simulator step is performed.
     *
     * @param mode
     * @param iteration
     */
    void simulatorStepped(Simulator::Mode mode, int iteration);


    /**
     * @brief Simulator finished simulation.
     *
     * @param mode
     */
    void simulatorFinished(Simulator::Mode mode, bool end);


    /**
     * @brief Cancel simulation initialization.
     */
    void simulatorInitializationCancel();


    /**
     * @brief Visualization layer toggle.
     *
     * @param flag
     */
    void visualizationLayerToggle(bool flag);


    /**
     * @brief A new message logged.
     *
     * @param type    Message type.
     * @param section Message section.
     * @param msg     Message to log.
     */
    void logAppend(Log::Type type, QString section, QString message);


// Public Operations
public:


    /**
     * @brief Set current file name.
     * @param filename
     */
    void setCurrentFile(QString filename);


    /**
     * @brief Open a file.
     * @param filename
     */
    void fileOpen(QString filename);


    /**
     * @brief Save current simulation file.
     * @param filename
     */
    void fileSave(QString filename);


// Private Operations
private:


    /**
     * @brief Store window settings.
     */
    void storeSettings() const;


    /**
     * @brief Restore window settings.
     */
    void restoreSettings();


    /**
     * @brief Initialize recent files actions.
     */
    void initRecentFiles();


    /**
     * @brief Update recent file actions.
     */
    void updateRecentFileActions();


    /**
     * @brief Initialize tools.
     */
    void initTools();


// Private Data Members
private:

    /// UI members.
    Ui::MainWindow* ui;

    /// Plugin manager.
    plugin::Manager m_pluginManager;

    /// Output log stream.
    LogStream m_logStream;

    /// Recent files action.
    QAction* m_recentFiles[MAX_RECENT_FILES];

    /// Current file name.
    QString m_filename;

    /// Previous window state.
    Qt::WindowStates m_previousState = Qt::WindowNoState;

    /// Background simulation thread.
    QThread m_simulatorThread;

    /// Simulator.
    Simulator m_simulator;

    /// Draw timer.
    QTimer m_simulatorDrawTimer;

    /// Actions for visualization layers.
    QList<QAction*> m_visualizationActions;

    /// Data plot export factory.
    DataExportPlotFactory m_dataExportFactory;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
