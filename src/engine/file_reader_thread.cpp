#include "file_reader_thread.h"

#include "algorithm"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

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
    QList<Record> records;
    records.reserve(_records.size());

    std::transform(_records.begin(),_records.end(),std::back_inserter(records),[](const auto& pair){
        return pair;
    });
    return records;
}

View::Record View::FileReaderThread::getRecord(const QString &file_name) const
{
    const QMutexLocker locker(&_mutex);
    if(auto it = _records.find(file_name); it != _records.end())
    {
        return it.value();
    }
    return {};
}

QString View::FileReaderThread::gerError(const QString &file_name) const
{
    const QMutexLocker locker(&_mutex);
    if(auto it = _errors.find(file_name); it != _errors.end())
    {
        return it.value();
    }
    return {};
}

void View::FileReaderThread::run()
{
    clear();
    QDir directory(_dir_path);
    QFileInfoList files = directory.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,QDir::NoSort);
    Q_EMIT filesToRead(files.count());
    int number{0};
    for (const QFileInfo &file_info : files)
    {
        ReadStatus status;
        QString file_name = file_info.fileName();
        std::unique_ptr<IFileReader> file_reader = _reader_factory->create(file_info.suffix());
        if(file_reader)
        {            
            Record rec;
            if(file_reader->read(file_info.absoluteFilePath(),rec))
            {                
                pushData(file_name,rec);
                status = ReadStatus::Success;
            }
            else
            {
                pushError(file_name,tr("Ошибка: %1").arg(file_reader->error()));
                status = ReadStatus::Error;
            }
        }
        else
        {
            pushError(file_name,tr("Тип расширения не поддерживается"));
            status = ReadStatus::Error;
        }
        Q_EMIT fileReaded(file_name,(int)status);
        Q_EMIT filesReaded(++number);
    }
}

void View::FileReaderThread::pushData(const QString &file_name, const Record &record)
{
    QMutexLocker locker(&_mutex);
    _records[file_name] = record;
}

void View::FileReaderThread::pushError(const QString &file_name, const QString &error)
{
    QMutexLocker locker(&_mutex);
    _errors[file_name] = error;
}

void View::FileReaderThread::clear()
{
    QMutexLocker locker(&_mutex);
    _records.clear();
    _errors.clear();
}

