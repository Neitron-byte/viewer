#include "controller.h"

#include <QUuid>
#include <QApplication>
#include <QDir>
#include <QDebug>

#include "readers/reader_factory.h"
#include "file_reader_thread.h"
#include "database/table.h"
#include "records_table_model.h"

#include "readers/json_file_reader.h"
#include "readers/xml_file_reader.h"

#include "database/connection.h"
#include "database/table_wrapper.h"

const QLatin1String data_base_name("editors.db");
const QLatin1String table_name("editors");

View::Controller::Controller(QSharedPointer<RecordsTableModel> records_model, QObject *parent): QObject(parent)
    , _records_model{records_model}
    , _reader_factory{std::make_shared<ReaderFactory>()}
    , _readers_thread{std::make_unique<FileReaderThread>(_reader_factory)}
{
    connect(_readers_thread.get(),&FileReaderThread::finished,this,&Controller::onFileReadingFinished);
    //connect(_readers_thread.get(),&FileReaderThread::message,this,&Controller::statusLoadedMessage);
    connect(_readers_thread.get(),&FileReaderThread::filesToRead,this,&Controller::filesCount);
    connect(_readers_thread.get(),&FileReaderThread::fileReaded,this,&Controller::onFileReaded);

    _reader_factory->registerCreator("xml",[](){ return std::make_unique<XMLFileReader>();});
    _reader_factory->registerCreator("json",[](){return std::make_unique<JsonFileReader>();});

    std::unique_ptr<Table> table = std::make_unique<Table>(table_name, Connection::createConnection(data_base_name));
    _database_is_connected = (table->isConnected() && table->create());
    _table = std::make_unique<TableWrapper>(std::move(table));

    connect(_records_model.get(),&RecordsTableModel::recordUpdated,this,&Controller::onRecordUpdated);

    reset();
}

void View::Controller::loadFiles(const QString &dir_path)
{
    reset();
    if(!_database_is_connected)
    {
        Q_EMIT message(tr("Отсутствует подключение к БД"));
        return;
    }
    Q_EMIT operationChanged(tr("Чтение файлов ..."));
    _readers_thread->setDir(dir_path);
    _readers_thread->start();
}

void View::Controller::loadData()
{
    if(!_database_is_connected)
    {
        Q_EMIT message(tr("Отсутствует подключение к БД"));
        return;
    }    
    _table->IDs([this](const QList<QString>& ids){
        for(const auto& id : ids)
        {
            _table->getRecord(id,[id,this](const Record& record){
                _records_model->append({id,record});
            });
        }
        Q_EMIT message(tr("Данные из базы успешно загружены"));
    });
}

void View::Controller::clearData()
{
    if(!_database_is_connected)
    {
        Q_EMIT message(tr("Отсутствует подключение к БД"));
        return;
    }
    _table->clear([this](bool result){
        if(result)
        {
            _records_model->clear();
            Q_EMIT message(tr("Данные в базе очищены"));
        }
        else
        {
            Q_EMIT message(tr("Данные не очищены. Ошибка: %1").arg(_table->error()));
        }
    });
}

void View::Controller::addRecord()
{
    QString uuid = uuidGenerate();
    Record rec;
    _table->append(uuid,rec,[this,uuid,rec](bool res){
        if(res)
        {
            _records_model->append({uuid,rec});
            Q_EMIT message(tr("Новая запись добавлена"));
        }
        else
        {
            Q_EMIT message(tr("Запись не добавлена. Ошибка: %1").arg(_table->error()));
        }
    });
}

void View::Controller::removeRecord(const QString &uuid)
{
    _table->remove(uuid,[this,uuid](bool res){
        if(res)
        {
            _records_model->removeRecord(uuid);
            Q_EMIT message(tr("Запись успешно удалена"));
        }
        else
        {
            Q_EMIT message(tr("Запись не удалена. Ошибка: %1").arg(_table->error()));
        }
    });
}

void View::Controller::onFileReadingFinished()
{
    Q_EMIT statusLoadedMessage(tr("Итого прочитано. Успешно: %1. C ошибками: %2").arg(_success_read_files.count()).arg(_error_list_files.count()));
    Q_EMIT operationChanged(tr("Загрузка в БД ..."));

    auto records = _readers_thread->getRecords();
    Q_EMIT filesCount(records.count());

    // for(const auto& rec : _readers_thread->getRecords())
    // {
    //     QString uuid = uuidGenerate();
    //     _records_model->append({uuid,rec});
    // }
    for(const auto& rec : _readers_thread->getRecords())
    {
        QString uuid = uuidGenerate();
        _table->append(uuid,rec,[this,uuid,rec](bool res){
            if(res)
            {
                _records_model->append({uuid,rec});
            }
            else
            {
                Q_EMIT statusLoadedMessage(_table->error());
            }
            progress(++_load_counter);
        });
    }
    Q_EMIT filesLoaded();
}

void View::Controller::onFileReaded(const QString &file_name, int status)
{
    if(status == ReadStatus::Error)
    {
        _error_list_files.append(file_name);
        Q_EMIT statusLoadedMessage(QString("Файл: %1 Ошибка: %2").arg(file_name).arg(_readers_thread->gerError(file_name)));
    }
    else
    {
        _success_read_files.append(file_name);
    }
    //qDebug() << "File Readed: " << file_name;
    // if(status == ReadStatus::Success)
    // {
    //     QString uuid = uuidGenerate();
    //     Record rec = _readers_thread->getRecord(file_name);
    //     _table->append(uuid,rec,[this,uuid,rec](bool res){
    //         if(res)
    //         {
    //             _records_model->append({uuid,rec});
    //         }
    //         else
    //         {
    //             qDebug() << _table->error();
    //         }
    //     });
    // }
    // else
    // {
    //     Q_EMIT statusLoadedMessage(QString("Файл: %1 Ошибка: %2").arg(file_name).arg(_readers_thread->gerError(file_name)));
    // }
    // Q_EMIT progress(++_counter);
}

void View::Controller::onRecordUpdated(const QString &uuid)
{
    auto record = _records_model->getRecord(uuid);
    _table->update(uuid,record,[this,uuid](bool result)
    {
        if(result)
            Q_EMIT message(tr("Запись успешно обновлена"));
        else
            Q_EMIT message(tr("Запись не обновлена. Ошибка: %1").arg(_table->error()));
    });
}

QString View::Controller::uuidGenerate()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void View::Controller::reset()
{
    _success_read_files.clear();
    _error_list_files.clear();
    _load_counter=0;
}
