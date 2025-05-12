#include "connection.h"

#include <QFile>
#include <QApplication>
#include <QDir>

QSqlDatabase View::Connection::createConnection(const QString &database_file_path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database_file_path);
    db.open();
    return db;
}
