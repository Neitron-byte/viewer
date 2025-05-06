#pragma once

#include "i_serialize.h"

namespace View
{
/*!
 * \brief The Record class
 * Структура записи
 */
class Record : public IJsonSerialize,IXmlSerialize
{
public:
    Record();
    ~Record() = default;

    bool isValid() const;

    void setEditorName(const QString& editor_name);
    void setFormats(const QStringList& formats);
    void setEncoding(const QStringList& encoding);
    void setHasIntelisense(bool has);
    void setHasPlugins(bool has);
    void setCanCompile(bool can);

    QString editorName() const {return _editor_name;}
    QStringList getFormats() const {return _formats;}
    QStringList getEncoding() const {return _encoding;}
    bool hasIntellisense() const {return _has_intellisense;}
    bool hasPlugins() const {return _has_plugins;}
    bool canCompile() const {return _can_compile;}

    // IXmlSerialize interface
public:
    QMap<QString, QString> asXml() const override;
    bool fromXml(const QMap<QString, QString> &map) override;

    // IJsonSerialize interface
public:
    QJsonObject asJson() const override;
    bool fromJson(const QJsonObject &object) override;

private:
    QString _editor_name;
    QStringList _formats;
    QStringList _encoding;
    bool _has_intellisense;
    bool _has_plugins;
    bool _can_compile;

    bool _is_set_intellisense;
    bool _is_set_plugins;
    bool _is_set_compile;
};
}
