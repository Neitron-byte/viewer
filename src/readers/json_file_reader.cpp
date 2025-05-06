#include "json_file_reader.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

#include "definitions.h"

bool View::JsonFileReader::read(const QString &file_path, Record &record)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        _error = file.errorString();
        return false;
    }
    QByteArray array = file.readAll();
    QJsonObject root = QJsonDocument::fromJson(array).object();

    if(!root.contains(rootTag))
    {
        _error = QObject::tr("Формат файла не поддерживается");
        return false;
    }

    record.fromJson(root.value(rootTag).toObject());
    file.close();
    return true;
}
