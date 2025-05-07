#include "file_reader_thread.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include "readers/reader_factory.h"
#include "readers/i_file_reader.h"


View::FileReaderThread::FileReaderThread(std::shared_ptr<ReaderFactory> reader_factory, QObject *parent) : QThread(parent)
    , _reader_factory(reader_factory)
{

}

void View::FileReaderThread::setDir(const QString &dir_path)
{
    QMutexLocker locker(&_mutex);
    _dir_path = dir_path;
}

QList<View::Record> View::FileReaderThread::getRecords() const
{
    const QMutexLocker locker(&_mutex);
    return _records;
}

void View::FileReaderThread::run()
{
    QDir directory(_dir_path);
    QFileInfoList files = directory.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,QDir::NoSort);
    Q_EMIT message(tr("Всего файлов: %1").arg(files.count()));
    int success = 0;
    int fail = 0;
    for (const QFileInfo &file_info : files)
    {
        IFileReader* file_reader = _reader_factory->createReader(file_info.suffix());
        if(file_reader)
        {
            Record rec;
            if(file_reader->read(file_info.absoluteFilePath(),rec))
            {
                pushData(rec);
                sendMessage(file_info.fileName(),"Успешно");
                ++success;
            }
            else
            {
                sendMessage(file_info.fileName(),tr("Ошибка: %1").arg(file_reader->error()));
                ++fail;
            }
        }
        else
        {
            sendMessage(file_info.fileName(),tr("Тип расширения не поддерживается"));
            ++fail;
        }
    }
}

void View::FileReaderThread::pushData(const Record &record)
{
    QMutexLocker locker(&_mutex);
    _records.append(record);
}

void View::FileReaderThread::sendMessage(const QString &file_name, const QString &msg)
{
    Q_EMIT message(tr("Файл: %1. Cтатус: %2").arg(file_name).arg(msg));
}

