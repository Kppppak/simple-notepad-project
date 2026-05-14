#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <QString>
#include <QFile>
#include <QTextStream>

#include <set>
#include <string>

class spell_checker
{
private:
    std::set<std::string> words;

public:
    void load_words(
        const QString& filename
    )
    {
        QFile file(filename);

        if(
            !file.open(
                QIODevice::ReadOnly |
                QIODevice::Text
            )
        )
        {
            return;
        }

        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString word =
                in.readLine()
                    .trimmed()
                    .toLower();

            words.insert(
                word.toStdString()
            );
        }
    }

    bool is_correct(
        const QString& word
    ) const
    {
        return words.contains(
            word.toLower()
                .toStdString()
        );
    }
};

#endif