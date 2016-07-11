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
#include <QDialog>

// CeCe
#include "cece/core/ViewPtr.hpp"

/* ************************************************************************ */

namespace Ui { class PlotCreateDialog; }

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

class DataExportPlotFactory;

/* ************************************************************************ */

/**
 * @brief Plot create dialog.
 */
class PlotCreateDialog : public QDialog
{
    Q_OBJECT

public:


    /**
     * @brief Constructor.
     * @param parent
     */
    explicit PlotCreateDialog(ViewPtr<DataExportPlotFactory> factory, QWidget* parent = nullptr);


    /**
     * @brief Destructor.
     */
    ~PlotCreateDialog();


// Public Accessors
public:


    /**
     * @brief Returns source exporter.
     *
     * @return
     */
    QString getSource() const noexcept;


    /**
     * @brief Returns axis X.
     *
     * @return
     */
    QString getAxisX() const noexcept;


    /**
     * @brief Returns axis Y.
     *
     * @return
     */
    QString getAxisY() const noexcept;


    /**
     * @brief Returns group name.
     *
     * @return
     */
    QString getGroup() const noexcept;


    /**
     * @brief Returns plot type.
     *
     * @return
     */
    QString getType() const noexcept;


// Public Slots
public slots:


    /**
     * @brief On source selected.
     * @param name
     */
    void on_comboBoxSource_currentIndexChanged(QString name);


    /**
     * @brief On axis x selected.
     * @param name
     */
    void on_comboBoxAxisX_currentIndexChanged(QString name);


    /**
     * @brief On axis y selected.
     * @param name
     */
    void on_comboBoxAxisY_currentIndexChanged(QString name);


    /**
     * @brief On group selected.
     * @param name
     */
    void on_comboBoxGroup_currentIndexChanged(QString name);


// Private Operations
private:


    /**
     * @brief Check if current state is valid.
     */
    void checkValidity();


// Private Data Members
private:

    /// UI members.
    Ui::PlotCreateDialog* ui;

    /// Factory.
    ViewPtr<DataExportPlotFactory> m_factory;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
