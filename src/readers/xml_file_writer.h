#pragma once
#include <QXmlStreamWriter>

#include "i_file_writer.h"

namespace View {
/*!
 * \brief The XMLFileWriter class
 * Писатель XML файлов
 */
class XMLFileWriter : public IFileWriter
{
public:
    XMLFileWriter() = default;
    ~XMLFileWriter() override = default;

    // IFileWriter interface
public:
    bool write(const QString& file_path,const Record& record) override;
    QString error() const override {return _error;}

private:
    QXmlStreamWriter xml;
    QString _error;
};
}
