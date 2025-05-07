#include "controller.h"

#include <QUuid>
#include <QApplication>
#include <QDir>

#include "readers/reader_factory.h"
#include "file_reader_thread.h"
#include "database/table.h"
#include "records_table_model.h"

#include "readers/json_file_reader.h"
#include "readers/xml_file_reader.h"

#include "database/connection.h"

const QLatin1String data_base_name("editors.db");
const QLatin1String table_name("editors");

View::Controller::Controller(QObject *parent): QObject(parent)
    , _reader_factory{std::make_shared<ReaderFactory>()}
    , _readers_thread{std::make_unique<FileReaderThread>(_reader_factory)}
    , _table{std::make_shared<Table>(table_name, Connection::createConnection(QApplication::applicationFilePath()+QDir::separator()+data_base_name))}
    , _records_model{new RecordsTableModel(_table,this)}
{
    connect(_readers_thread.get(),&FileReaderThread::finished,this,&Controller::onLoadThreadFinished);
    connect(_readers_thread.get(),&FileReaderThread::message,this,&Controller::statusLoadedMessage);
    connect(_readers_thread.get(),&FileReaderThread::filesCount,this,&Controller::filesCount);
    connect(_readers_thread.get(),&FileReaderThread::filesLoaded,this,&Controller::progress);
    _reader_factory->registerCreator("xml",[](){ return std::unique_ptr<XMLFileReader>();});
    _reader_factory->registerCreator("json",[](){return std::unique_ptr<JsonFileReader>();});
}

void View::Controller::loadFiles(const QString &dir_path)
{
    _readers_thread->setDir(dir_path);
    _readers_thread->start();
}

View::RecordsTableModel *View::Controller::getModel() const
{
    return _records_model;
}

void View::Controller::onLoadThreadFinished()
{
    auto records = _readers_thread->getRecords();

    for(const auto& rec : records)
    {
        _records_model->appendData({{uuidGenerate(),rec}});
    }

    Q_EMIT filesLoaded();
}

QString View::Controller::uuidGenerate()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
