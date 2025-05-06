#pragma once

#include <QJsonObject>

namespace View
{
/*!
 * \brief The IJsonSerialize class
 * Интерфейс Json сериализации
 */
class IJsonSerialize
{
public:
    virtual ~IJsonSerialize() = default;

    virtual QJsonObject asJson() const = 0;
    virtual bool fromJson(const QJsonObject& object) = 0;
};


/*!
 * \brief The IXmlSerialize class
 * Интерфейс для Xml сериализации
 */
class IXmlSerialize
{
public:
    virtual ~IXmlSerialize() = default;

    virtual QMap<QString,QString> asXml() const = 0;
    virtual bool fromXml(const QMap<QString,QString>& map) = 0;
};
}
