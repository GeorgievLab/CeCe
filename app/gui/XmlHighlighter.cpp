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
#include "XmlHighlighter.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

XmlHighlighter::XmlHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    // Element
    {
        QRegExp regexStart("(<[\\w|-]+)|(</[\\w|-]+>)");
        QRegExp regexEnd("/?>");
        QTextCharFormat format;
        format.setForeground(Qt::darkMagenta);
        format.setFontWeight(QFont::Bold);
        m_highlightRules.append({regexStart, format});
        m_highlightRules.append({regexEnd, format});
    }

    // Attribute
    {
        QRegExp regex("[\\w|-]+=");
        QTextCharFormat format;
        format.setForeground(Qt::darkGreen);
        format.setFontWeight(QFont::Bold);
        format.setFontItalic(true);
        m_highlightRules.append({regex, format});
    }

    // Value
    {
        QRegExp regex("\"[^\\n\"]*\"");
        QTextCharFormat format;
        format.setForeground(Qt::darkRed);
        m_highlightRules.append({regex, format});
    }

    // Comment
    {
        QRegExp regex("<!--.*-->");
        QTextCharFormat format;
        format.setForeground(Qt::gray);
        m_highlightRules.append({regex, format});
    }

    // Declaration
    {
        QRegExp regex("<\\?xml.+\\?>");
        QTextCharFormat format;
        format.setForeground(Qt::gray);
        m_highlightRules.append({regex, format});
    }
}

/* ************************************************************************ */

void XmlHighlighter::highlightBlock(const QString& text)
{
    for (const HighlightRule& rule : m_highlightRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
