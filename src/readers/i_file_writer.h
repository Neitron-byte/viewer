#pragma once
#include <QIODevice>

#include "data/record.h"

namespace View
{
/*!
 * \brief The IWriter class
 * Интерфейс писателя данных
 */
class IFileWriter
{
public:
    virtual ~IFileWriter() = default;
    virtual bool write(const QString& file_path,const Record& record) = 0;
    virtual QString error() const = 0;
};
}
