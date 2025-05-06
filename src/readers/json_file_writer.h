#pragma once

#include "i_file_writer.h"

namespace View
{
/*!
 * \brief The JsonFileWriter class
 * Писать Json файлов
 */
class JsonFileWriter : public IFileWriter
{
public:
    JsonFileWriter() = default;
    ~JsonFileWriter() override = default;

    // IFileWriter interface
public:
    bool write(const QString &file_path, const Record &record) override;
    QString error() const override {return _error;}

private:
    QString _error;
};
}
