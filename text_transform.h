#ifndef TEXT_TRANSFORM_H
#define TEXT_TRANSFORM_H

#include <QString>

class text_transform
{
public:
    static QString to_upper(
        const QString &text
    )
    {
        return text.toUpper();
    }

    static QString to_lower(
        const QString &text
    )
    {
        return text.toLower();
    }

    static QString capitalize(
        const QString &text
    )
    {
        QString result = text.toLower();

        if (!result.isEmpty())
        {
            result[0] = result[0].toUpper();
        }

        return result;
    }
};

#endif
