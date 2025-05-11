#include "table.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>

#include "utils.h"

View::Table::Table(const QString &table_name, QSqlDatabase connection) :
    _table_name(table_name), _connection(connection)
{

}

View::Table::~Table()
{
    if (_connection.isOpen())
    {
        _connection.close();
    }        
}

bool View::Table::append(const QString &id, const Record &rec)
{
    QSqlQuery insert_query(_connection);
    insert_query.prepare(QString("INSERT INTO %1 (id, name, formats, encoding, intellisense, plugins, compile) "
                  "VALUES (:id, :name, :formats, :encoding, :intellisense, :plugins, :compile)").arg(_table_name));

    insert_query.bindValue(0, id);
    insert_query.bindValue(1, rec.editorName());
    insert_query.bindValue(2, rec.getFormats());
    insert_query.bindValue(3, rec.getEncoding());
    insert_query.bindValue(4, toInt(rec.hasIntellisense()));
    insert_query.bindValue(5, toInt(rec.hasPlugins()));
    insert_query.bindValue(6, toInt(rec.canCompile()));
    return insert_query.exec();
}

bool View::Table::update(const QString &id, const Record &rec)
{   
    QString update_query =
        QString("UPDATE %1 SET name = %2, formats = %3, encoding = '%4', intellisense = '%5', plugins = '%6', compile = '%7' WHERE id = '%8'")
            .arg(_table_name)
            .arg(rec.editorName())
            .arg(rec.getFormats())
            .arg(rec.getEncoding())
            .arg(toInt(rec.hasIntellisense()))
            .arg(toInt(rec.hasPlugins()))
            .arg(toInt(rec.canCompile()))
            .arg(id);

    return execQuery(update_query);
}

bool View::Table::remove(const QString &id)
{
    return execQuery(QString("DELETE FROM %1 WHERE id = '%2'").arg(_table_name).arg(id));
}

bool View::Table::contains(const QString &id) const
{
    QSqlQuery select_query(_connection);
    if (!select_query.exec(
        QString("SELECT id FROM %1 WHERE id = '%2'").arg(_table_name).arg(id)))
    {
        return false;
    }

    if (select_query.size() > 0)
    {
        return true;
    }
    return false;
}

View::Record View::Table::getRecord(const QString &id) const
{
    QSqlQuery select_query(_connection);
    select_query.exec(QString("SELECT * FROM %1 WHERE id = '%2'")
                               .arg(_table_name)
                               .arg(id));

    Record rec;

    while (select_query.next())
    {
        rec.setEditorName(select_query.value("name").toString());
        rec.setFormats(select_query.value("formats").toString());
        rec.setEncoding(select_query.value("encoding").toString());
        bool ok = true;
        bool res = toBool(select_query.value("intellisense").toInt(),&ok);
        if(ok)
            rec.setHasIntelisense(res);
        res = toBool(select_query.value("plugins").toInt(),&ok);
        if(ok)
            rec.setHasPlugins(res);
        res = toBool(select_query.value("compile").toInt(),&ok);
        if(ok)
            rec.setCanCompile(res);
    }
    return rec;
}

QList<QString> View::Table::IDs() const
{
    QList<QString> ids; 

    QSqlQuery select_query(_connection);
    select_query.exec(
        QString("SELECT id FROM %1").arg(_table_name));

    while (select_query.next())
    {
        QString id = select_query.value(0).toString();
        if (!id.isEmpty())
            ids << id;
    }
    return ids;    
}

int View::Table::count() const
{
    QSqlQuery count_query(_connection);
    count_query.exec(QString("SELECT COUNT(*) FROM %1;").arg(_table_name));
    if(count_query.next())
    {
        return count_query.value(0).toInt();
    }
    return -1;
}

bool View::Table::create() const
{
    auto create =
        QString("CREATE TABLE IF NOT EXISTS editors"
                          "( id TEXT NOT NULL, name TEXT, formats TEXT, "
                          "encoding TEXT, intellisense INTEGER CHECK (intellisense IN (0, 1)),"
                          "plugins INTEGER CHECK (plugins IN (0, 1)), "
                          "compile INTEGER CHECK (compile IN (0, 1)));")
            .arg(_table_name);
    QSqlQuery query(_connection);
    return query.exec(create);
}

bool View::Table::drop() const
{
    QString drop_query = QString("DROP TABLE %1").arg(_table_name);
    return execQuery(drop_query);
}

QString View::Table::error() const
{
    return _connection.lastError().text();
}

bool View::Table::clear()
{
    QString query = QString("DELETE FROM %1").arg(_table_name);
    return execQuery(query);
}

bool View::Table::isConnected() const
{
    return _connection.isOpen();
}

bool View::Table::execQuery(const QString &query) const
{
    QSqlQuery q(_connection);
    return q.exec(query);
}
