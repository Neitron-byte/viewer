#pragma once
#include <memory>

#include <QThread>
#include <QList>
#include <QString>
#include <QMutex>

#include "data/record.h"

namespace View {

class ReaderFactory;
/*!
 * \brief The FileReaderThread class
 * Поток чтения файлов
 */
class FileReaderThread : public QThread
{
    Q_OBJECT
public:
    FileReaderThread(std::shared_ptr<ReaderFactory> reader_factory, QObject* parent = nullptr);

    QList<Record> getRecords() const;

    void setDir(const QString& dir_path);

signals:
    void message(const QString& msg);
    void filesCount(int num);
    void filesLoaded(int num);

    // QThread interface    
protected:
    void run() override;

private:
    void pushData(const Record& record);

    void sendMessage(const QString& file_name, const QString& msg);    

private:
    mutable QMutex _mutex;

    QList<Record> _records;

    QString _dir_path;

    std::shared_ptr<ReaderFactory> _reader_factory;
};
}

