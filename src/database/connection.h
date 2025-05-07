#pragma once
#include <QString>
#include <QSqlDatabase>

namespace View
{
class Connection
{
public:
    static QSqlDatabase createConnection(const QString & database_file_path);

private:
    Connection() = default;
};
}
