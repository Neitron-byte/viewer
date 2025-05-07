#pragma once
#include <memory>

#include <QObject>
#include <QString>

#include "file_reader_thread.h"

namespace View
{
class ReaderFactory;
//class FileReaderThread;
class Table;
class RecordsTableModel;

/*!
 * \brief The Controller class
 */
class Controller : public QObject
{
Q_OBJECT
public:
    Controller(QObject* parent = nullptr);

    void loadFiles(const QString& dir_path);

    RecordsTableModel* getModel() const;

signals:
    void filesLoaded();
    void statusLoadedMessage(const QString& msg);
    void filesCount(int num);
    void progress(int num);

private slots:
    void onLoadThreadFinished();

private:
    QString uuidGenerate();

private:
    std::shared_ptr<ReaderFactory> _reader_factory;
    std::unique_ptr<FileReaderThread> _readers_thread;
    std::shared_ptr<Table> _table;
    RecordsTableModel* _records_model;
};
}
