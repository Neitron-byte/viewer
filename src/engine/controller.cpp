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

View::Controller::Controller(QObject *parent): QObject(parent)
    , _reader_factory{std::make_shared<ReaderFactory>()}
    , _readers_thread{std::make_unique<FileReaderThread>(_reader_factory)}
    , _records_model{new RecordsTableModel(this)}
{
    connect(_readers_thread.get(),&FileReaderThread::finished,this,&Controller::onLoadThreadFinished);
    connect(_readers_thread.get(),&FileReaderThread::message,this,&Controller::statusLoadedMessage);
    connect(_readers_thread.get(),&FileReaderThread::filesCount,this,&Controller::filesCount);
    connect(_readers_thread.get(),&FileReaderThread::filesLoaded,this,&Controller::progress);

    _reader_factory->registerCreator("xml",[](){ return std::make_unique<XMLFileReader>();});
    _reader_factory->registerCreator("json",[](){return std::make_unique<JsonFileReader>();});

    std::unique_ptr<Table> table = std::make_unique<Table>(table_name, Connection::createConnection(data_base_name));
    _database_is_connected = (table->isConnected() && table->create());
    _table = std::make_unique<TableWrapper>(std::move(table));
}

void View::Controller::loadFiles(const QString &dir_path)
{
    if(!_database_is_connected)
    {
        Q_EMIT message(tr("Отсутствует подключение к БД"));
        return;
    }
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

View::RecordsTableModel *View::Controller::getModel() const
{
    return _records_model;
}

void View::Controller::onLoadThreadFinished()
{    
    if(!_database_is_connected)
    {
        Q_EMIT message(tr("Ошибка загрузки данных. Отсутствует подключение к базе"));
        return;
    }
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
                qDebug() << _table->error();
            }
        });
    }
    Q_EMIT filesLoaded();
}

QString View::Controller::uuidGenerate()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
