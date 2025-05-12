#pragma once
#include <QString>
#include <QSqlDatabase>

namespace View
{
/*!
 * \brief The Connection class
 */
class Connection
{
public:
    /// Метод получения подключения к БД
    static QSqlDatabase createConnection(const QString & database_file_path);

private:
    Connection() = default;
};
}
