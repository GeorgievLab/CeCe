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

namespace Ui { class PlotCustomizationDialog; }

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief Plot customization dialog.
 */
class PlotCustomizationDialog : public QDialog
{
    Q_OBJECT

public:


    /**
     * @brief Constructor.
     * @param parent
     */
    explicit PlotCustomizationDialog(QWidget* parent = nullptr);


    /**
     * @brief Destructor.
     */
    ~PlotCustomizationDialog();


// Public Accessors & Mutators
public:


    /**
     * @brief Returns plot title.
     *
     * @return
     */
    QString getPlotTitle() const noexcept;


    /**
     * @brief Set plot title.
     *
     * @param title
     */
    void setPlotTitle(QString title) noexcept;


    /**
     * @brief Returns plot X label.
     *
     * @return
     */
    QString getPlotXLabel() const noexcept;


    /**
     * @brief Set plot X label.
     *
     * @param title
     */
    void setPlotXLabel(QString label) noexcept;


    /**
     * @brief Returns plot Y label.
     *
     * @return
     */
    QString getPlotYLabel() const noexcept;


    /**
     * @brief Set plot Y label.
     *
     * @param title
     */
    void setPlotYLabel(QString label) noexcept;


// Private Data Members
private:

    /// UI members.
    Ui::PlotCustomizationDialog* ui;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
