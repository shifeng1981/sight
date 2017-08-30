/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_HIGHLIGHTER_PYTHONHIGHLIGHTER_HPP__
#define __FWGUIQUICK_HIGHLIGHTER_PYTHONHIGHLIGHTER_HPP__

#include "fwGuiQuick/config.hpp"

#include <QHash>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

namespace fwGuiQuick
{
namespace highlighter
{

class FWGUIQUICK_CLASS_API PythonHighlighter : public QSyntaxHighlighter
{
Q_OBJECT

public:
    FWGUIQUICK_API PythonHighlighter(QTextDocument* parent = 0);

protected:

    FWGUIQUICK_API void highlightBlock(const QString& text);

private:

    class HighlightingRule
    {
    public:
        HighlightingRule() :
            nth(0)
        {
        }
        int nth;
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat multiLineCommentFormat;

};

} //namespace fwGuiQuick
} //namespace highlighter

#endif // __FWGUIQUICK_HIGHLIGHTER_PYTHONHIGHLIGHTER_HPP__
