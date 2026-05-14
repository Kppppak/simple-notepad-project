#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <QString>
#include <QFile>
#include <QTextStream>

#include <set>
#include <vector>
#include <string>

class spell_checker
{
private:
    std::set<std::string> words;

public:
    void load_words(const QString& filename)
    {
        QFile file(filename);

        if (!file.open(
                QIODevice::ReadOnly |
                QIODevice::Text))
        {
            return;
        }

        QTextStream in(&file);

        while (!in.atEnd())
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

    std::vector<QString> suggestions(
        const QString& word
    ) const
    {
        std::vector<QString> result;

        QString prefix =
            word.left(2).toLower();

        for (const auto& w : words)
        {
            QString current =
                QString::fromStdString(w);

            if (
                current.startsWith(
                    prefix
                )
            )
            {
                result.push_back(
                    current
                );
            }

            if (
                result.size() == 5
            )
            {
                break;
            }
        }

        return result;
    }
};

#endif