#include "file_writer.h"

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QPair>

#include <QFileInfo>

View::FileWriter::FileWriter(std::shared_ptr<WriterFactory> writer_factory, QObject *parent) :QObject(parent)
    , _writer_factory{writer_factory}
{

}

void View::FileWriter::write(const QString &file_path, const Record &record)
{
    QFutureWatcher<QPair<bool,QString>> *watcher = new QFutureWatcher<QPair<bool,QString>>(this);

    connect(watcher, &QFutureWatcher<QPair<bool,QString>>::finished, [watcher, this]() {
        QPair<bool,QString> res = watcher->result();
        if (res.first)
        {
            Q_EMIT writeSucceeded();
        }
        else
        {
            _error = res.second;
            Q_EMIT writeFailed();
        }
        watcher->deleteLater();
    });

    QFuture<QPair<bool,QString>> future = QtConcurrent::run([file_path, record, factory = _writer_factory]() ->QPair<bool,QString> {
        QFileInfo file_info(file_path);
        std::unique_ptr<IFileWriter> file_writer = factory->create(file_info.suffix());
        if(file_writer->write(file_path,record))
        {
            return {true,QString()};
        }
        else
        {
            return {false,file_writer->error()};
        }
    });

    watcher->setFuture(future);
}
