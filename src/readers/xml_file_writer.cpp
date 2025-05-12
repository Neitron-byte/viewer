#include "xml_file_writer.h"

#include <QFile>

#include "definitions.h"


bool View::XMLFileWriter::write(const QString &file_path, const Record &record)
{
    QFile file(file_path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        _error = file.errorString();
        return false;
    }

    xml.setDevice(&file);

    xml.writeStartDocument();
    xml.writeStartElement(rootTag);

    auto map = record.asXml();

    for(auto it = map.begin(),it2 = map.end();it != it2; ++it)
    {
        //xml.writeTextElement(it.key(),it.value());
        xml.writeStartElement(it.key());
        xml.writeCharacters(it.value());
        xml.writeEndElement();
    }

    //root
    xml.writeEndElement();
    xml.writeEndDocument();
    return true;
}
