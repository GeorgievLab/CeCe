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

/* ************************************************************************ */

namespace Ui
{
class MainWindow;
}

/* ************************************************************************ */

/**
 * @brief Main GUI window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

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
     * @brief Toggle toolbar visibility.
     */
    void viewToolbar(bool);

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

private:
    /// UI members.
    Ui::MainWindow* ui;

    /// Current file name.
    QString m_filename;

    /// Previous window state.
    Qt::WindowStates m_previousState = Qt::WindowNoState;
};

/* ************************************************************************ */
