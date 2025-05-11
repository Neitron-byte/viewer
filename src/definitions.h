#pragma once
#include <QString>
#include <QObject>

namespace View
{
Q_NAMESPACE
/// root тег
const QLatin1String rootTag("root");

/// Статус чтения файла
enum ReadStatus
{
    Success = 0,
    Error
};
Q_ENUM_NS(ReadStatus)
}
