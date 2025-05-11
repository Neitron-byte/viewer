#pragma once
#include <memory>

#include <QThread>
#include <QList>
#include <QString>
#include <QMutex>
#include <QMap>

#include "data/record.h"

namespace View {

class ReaderFactory;
class IFileReader;
/*!
 * \brief The FileReaderThread class
 * Поток чтения файлов
 */
class FileReaderThread : public QThread
{
    Q_OBJECT
public:

    enum ReadStatus
    {
        Success = 0,
        Error
    };

    FileReaderThread(std::shared_ptr<ReaderFactory> reader_factory, QObject* parent = nullptr);

    QList<Record> getRecords() const;

    Record getRecord(const QString& file_name) const;

    QString gerError(const QString& file_name) const;

    void setDir(const QString& dir_path);

signals:
    void filesToRead(int num);
    void fileReaded(const QString& file_name, ReadStatus status);
    void filesReaded(int num);
    void fileReadingFinished();
    // QThread interface    
protected:
    void run() override;

private:
    void pushData(const QString& file_name, const Record& record);

    void pushError(const QString& file_name, const QString& error);

    std::unique_ptr<IFileReader> getReader(const QString& extansion);

    void clear();

private:
    mutable QMutex _mutex;

    QMap<QString,Record> _records;
    QMap<QString,QString> _errors;

    QString _dir_path;

    std::shared_ptr<ReaderFactory> _reader_factory;
};
}

