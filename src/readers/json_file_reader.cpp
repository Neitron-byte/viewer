#include "json_file_reader.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

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
    QJsonParseError parse_error{QJsonParseError::NoError};
    QJsonObject root = QJsonDocument::fromJson(array,&parse_error).object();

    if(parse_error.error != QJsonParseError::NoError)
    {
        _error = QObject::tr("Ошибка парсинга Json: %1. Позиция: %2").arg(parse_error.errorString()).arg(parse_error.offset);
        return false;
    }
    if(!root.contains(rootTag))
    {
        _error = QObject::tr("Формат файла не поддерживается");
        return false;
    }
    record.fromJson(root.value(rootTag).toObject());
    file.close();
    return true;
}
