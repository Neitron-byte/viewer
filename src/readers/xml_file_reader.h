#pragma once
#include <QXmlStreamReader>

#include "i_file_reader.h"

namespace View {
/*!
 * \brief The XMLFileReader class
 * Читальщик XML файлов
 */
class XMLFileReader : public IFileReader
{
public:
    XMLFileReader() = default;
    ~XMLFileReader() override = default;

    // IReader interface
public:
    bool read(const QString& file_path,Record& record) override;
    QString error() const override {return _error;}

private:
    void read(Record& record);

private:
    QXmlStreamReader xml;
    QString _error;
};
}
