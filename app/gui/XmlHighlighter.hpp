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
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QString>
#include <QTextCharFormat>
#include <QRegExp>

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief XML highlighter
 *
 * @see Based on https://github.com/d1vanov/basic-xml-syntax-highlighter
 */
class XmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param parent
     */
    explicit XmlHighlighter(QTextDocument* parent);


// Protected Operations
protected:


    /**
     * @brief Highlight block.
     *
     * @param text
     */
    void highlightBlock(const QString& text) override;


// Private Data Members
private:


    /// Highlight Rule
    struct HighlightRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    /// Highlight rules.
    QVector<HighlightRule> m_highlightRules;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
