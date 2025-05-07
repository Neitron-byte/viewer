#pragma once
#include <QSqlDatabase>

#include "i_table.h"

namespace View
{
/*!
 * \brief The Table class
 * Таблица в базе данных
 */
class Table : public ITable
{
public:
    Table(const QString &table_name, QSqlDatabase connection);
    ~Table() override;

    bool create() const override;
    bool drop() const override;

    bool append(const QString &id, const Record &rec) override;
    bool update(const QString &id, const Record &rec) override;
    bool remove(const QString &id) override;
    bool contains(const QString &id) const override;
    Record getRecord(const QString &id) const override;
    QList<QString> IDs() const override;

    int count() const override;
    
    QString error() const override;

    bool clear() override;

    bool isConnected() const;

private:
    bool execQuery(const QString &query) const;

private:
    QString _table_name;
    QSqlDatabase _connection;
};
}
