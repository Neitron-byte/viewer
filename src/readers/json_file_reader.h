#pragma once

#include "i_file_reader.h"

namespace View
{
/*!
 * \brief The JsonFileReader class
 * Читальщик Json файлов
 */
class JsonFileReader : public IFileReader
{
public:
    JsonFileReader() = default;
    ~JsonFileReader() override = default;

    // IFileReader interface
public:
    bool read(const QString &file_path, Record &record) override;
    QString error() const override {return _error;}

private:
    QString _error;
};
}
