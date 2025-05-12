#include "xml_file_reader.h"

#include <QFile>

#include "definitions.h"

bool View::XMLFileReader::read(const QString &file_path, Record& record)
{
    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        _error = file.errorString();
        return false;
    }

    xml.setDevice(&file);

    if(xml.readNextStartElement())
    {
        if (xml.name() == rootTag)
        {
            read(record);
            return true;
        }
        else
        {
            xml.raiseError(
                QObject::tr("Формат файла не поддерживается"));
        }
    }
    file.close();
    if(xml.hasError())
    {
        _error = xml.errorString();
    }
    return !xml.error();
}

void View::XMLFileReader::read(Record& record)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == rootTag);
    QMap<QString,QString> tmp;
    while (xml.readNextStartElement())
    {        
        tmp[xml.name().toString()] =xml.readElementText();
    }
    record.fromXml(tmp);
}
