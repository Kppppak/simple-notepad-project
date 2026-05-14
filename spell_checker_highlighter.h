#ifndef SPELL_CHECKER_HIGHLIGHTER_H
#define SPELL_CHECKER_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

#include "spell_checker.h"

class spell_checker_highlighter
    : public QSyntaxHighlighter
{
private:
    spell_checker& checker;

public:
    spell_checker_highlighter(
        QTextDocument* parent,
        spell_checker& checker_ref
    )
        : QSyntaxHighlighter(parent),
          checker(checker_ref)
    {
    }

protected:
    void highlightBlock(
        const QString& text
    ) override
    {
        QTextCharFormat format;

        format.setUnderlineStyle(
            QTextCharFormat::SpellCheckUnderline
        );

        format.setUnderlineColor(
            Qt::red
        );

        QRegularExpression regex(
            "\\b[A-Za-z]+\\b"
        );

        auto iterator =
            regex.globalMatch(text);

        while(iterator.hasNext())
        {
            auto match =
                iterator.next();

            QString word =
                match.captured();

            if(
                !checker.is_correct(
                    word
                )
            )
            {
                setFormat(
                    match.capturedStart(),
                    match.capturedLength(),
                    format
                );
            }
        }
    }
};

#endif