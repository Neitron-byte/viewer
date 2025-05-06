#pragma once
#include <QIODevice>

#include "data/record.h"

namespace View
{
/*!
 * \brief The IReader class
 * Интерфейс читальщика данных
 */
class IFileReader
{
public:
    virtual ~IFileReader() = default;
    virtual bool read(const QString& file_path,Record& record) = 0;
    virtual QString error() const = 0;
};
}
