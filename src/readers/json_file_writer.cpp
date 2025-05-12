#include "json_file_writer.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

#include "definitions.h"

bool View::JsonFileWriter::write(const QString &file_path, const Record &record)
{
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        _error = file.errorString();
        return false;
    }
    QJsonObject obj;
    obj[rootTag] = record.asJson();
    file.write(QJsonDocument(obj).toJson());
    file.close();
    return true;
}
