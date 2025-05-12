#pragma once

#include <QObject>

#include "data/record.h"

#include "readers/factory.h"

namespace View {
/*!
 * \brief The FileWriter class
 * Экспортер записей
 */
class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(std::shared_ptr<WriterFactory> writer_factory, QObject *parent = nullptr);

    void write(const QString &file_path, const Record &record);

    QString error() const {return _error;}

signals:
    void writeSucceeded();
    void writeFailed();

private:
    std::shared_ptr<WriterFactory> _writer_factory;
    QString _error;
};
}
