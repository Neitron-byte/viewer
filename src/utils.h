#pragma once
#include <QString>
#include <QList>

const QLatin1Char separator(';');

const QLatin1String true_str("true");
const QLatin1String false_str("false");

namespace View
{
inline QStringList toList(const QString& str)
{
    return str.split(separator,Qt::SkipEmptyParts);
}

inline QString toString(const QStringList& list)
{
    return list.join(separator);
}

inline QString toString(bool value)
{
    if(value)
        return true_str;
    return false_str;
}

inline bool toBool(const QString& bool_str)
{
    if(bool_str == true_str)
        return true;
    return false;
}

inline int toInt(bool b)
{
    return b == true ? 1 : 0;
}

inline bool toBool(int num, bool* ok)
{
    if(num == 0)
    {
        *ok = true;
        return false;
    }
    else if(num == 1)
    {
        *ok = true;
        return true;
    }
    *ok = false;
    return false;
}

}
