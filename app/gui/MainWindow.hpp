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

#pragma once

/* ************************************************************************ */

// Qt
#include <QMainWindow>
#include <QThread>
#include <QTreeWidget>
#include <QTimer>

// GUI
#include "LogStream.hpp"
#include "Simulator.hpp"

/* ************************************************************************ */

namespace Ui
{
class MainWindow;
}

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
    void fileNew();


    /**
     * @brief Open an existing simulation file.
     */
    void fileOpen();


    /**
     * @brief Save current simulation file.
     */
    void fileSave();


    /**
     * @brief Save current simulation file as.
     */
    void fileSaveAs();


    /**
     * @brief Open recent file.
     */
    void fileRecentOpen();


    /**
     * @brief Toggle fullscreen mode.
     */
    void viewFullscreen(bool);


    /**
     * @brief Start simulation.
     */
    void simulationStart();


    /**
     * @brief Pause simulation.
     */
    void simulationPause();


    /**
     * @brief Step simulation.
     */
    void simulationStep();


    /**
     * @brief Reset simulace.
     */
    void simulationReset();


    /**
     * @brief Show about dialog.
     */
    void helpAbout();


    /**
     * @brief If simulator is running.
     * @param flag
     */
    void simulatorRunning(bool flag);


    /**
     * @brief If simulator contains valid simulation.
     * @param flag
     */
    void simulatorLoaded(bool flag);


    /**
     * @brief On simulator error.
     * @param message
     */
    void simulatorError(QString message);


    /**
     * @brief A simulator step is performed.
     * @param iteration
     * @param iterations
     */
    void simulatorStepped(int iteration, int iterations);


    /**
     * @brief Simulator finished simulation.
     *
     * @param end
     */
    void simulatorFinished(bool end);


    /**
     * @brief Selected item from tree.
     * @param item
     * @param column
     */
    void editTreeItemSelected(QTreeWidgetItem* item, int column);


// Protected Operations
protected:


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
     * @brief Initialize simulator.
     */
    void initSimulator();


    /**
     * @brief Initialize simulation tree.
     */
    void initSimulation();


// Private Data Members
private:

    /// UI members.
    Ui::MainWindow* ui;

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

    /// Plugins tree item.
    QTreeWidgetItem* m_pluginsItem;

    /// Modules tree item.
    QTreeWidgetItem* m_modulesItem;

    /// Objects tree item.
    QTreeWidgetItem* m_objectsItem;

    /// Programs tree item.
    QTreeWidgetItem* m_programsItem;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
