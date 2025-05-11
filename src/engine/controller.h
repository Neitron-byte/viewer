#pragma once
#include <memory>

#include <QObject>
#include <QString>

#include "file_reader_thread.h"
#include "database/table_wrapper.h"

namespace View
{
class ReaderFactory;
//class FileReaderThread;
//class TableWrapper;
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

public slots:
    void loadData();
    void clearData();

signals:
    void filesLoaded();
    void statusLoadedMessage(const QString& msg);
    void filesCount(int num);
    void progress(int num);
    void message(const QString& message);
    void operationChanged(const QString& operation_name);

private slots:
    void onFileReadingFinished();
    void onFileReaded(const QString& file_name, int status);

private:
    QString uuidGenerate();

    void reset();

private:
    std::shared_ptr<ReaderFactory> _reader_factory;
    std::unique_ptr<FileReaderThread> _readers_thread;
    RecordsTableModel* _records_model;
    bool _database_is_connected;
    std::unique_ptr<TableWrapper> _table;

    QList<QString> _success_read_files;
    QList<QString> _error_list_files;
    int _load_counter;
};
}
