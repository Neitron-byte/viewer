#pragma once
#include <memory>
#include <functional>

#include <QObject>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QMutex>

#include "data/record.h"
#include "table.h"

namespace View
{
/*!
 * \brief The TableWrapper class
 * Обертка над таблицей для выполнения асинхронных запросов
 */
class TableWrapper : public QObject
{
Q_OBJECT
public:
    TableWrapper(std::unique_ptr<Table> table, QObject* parent = nullptr);

    using boolHandler = std::function<void(bool)>;
    using listHandler = std::function<void(QList<QString>)>;
    using countHandler = std::function<void(int)>;
    using recordHandler = std::function<void(Record)>;

    void create(boolHandler callback);
    void drop(boolHandler callback);

    void append(const QString &id, const Record &rec, boolHandler callback);
    void update(const QString &id, const Record &rec, boolHandler callback);
    void remove(const QString &id, boolHandler callback);
    void contains(const QString &id, boolHandler callback) const;
    void getRecord(const QString &id, recordHandler callback) const;
    void IDs(listHandler callback) const;

    void count(countHandler callback) const;

    QString error() const;

    void clear(boolHandler callback);

    bool isConnected() const;

private:
    template<typename ResultType, typename Function>
    void executeAsync(Function func, std::function<void(ResultType)> callback) const;

private:
    mutable QMutex _mutex;
    std::unique_ptr<Table> _table;
};

template<typename ResultType, typename Function>
inline void TableWrapper::executeAsync(Function func, std::function<void (ResultType)> callback) const
{
    auto watcher = new QFutureWatcher<ResultType>();
    QObject::connect(watcher, &QFutureWatcher<ResultType>::finished, [watcher, callback]() {
        ResultType result = watcher->result();
        callback(result);
        watcher->deleteLater();
    });

    QFuture<ResultType> future = QtConcurrent::run(func);
    watcher->setFuture(future);
}
}
