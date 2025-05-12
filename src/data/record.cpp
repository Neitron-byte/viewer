#include "record.h"

#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>
#include <QVariantList>

#include "utils.h"

const QLatin1String editor_key("texteditor");
const QLatin1String formats_key("fileformats");
const QLatin1String encoding_key("encoding");
const QLatin1String intellisense_key("hasintellisense");
const QLatin1String plugins_key("hasplugins");
const QLatin1String compile_key("cancompile");

const QStringList keys{editor_key,formats_key,encoding_key,intellisense_key,plugins_key,compile_key};

View::Record::Record()
{
    _has_intellisense = false;
    _has_plugins = false;
    _can_compile = false;

    _is_set_intellisense = false;
    _is_set_plugins = false;
    _is_set_compile = false;
}

bool View::Record::isValid() const
{
    return !_editor_name.isEmpty() && !_formats.isEmpty() && !_encoding.isEmpty()
           && _is_set_intellisense && _is_set_plugins && _is_set_compile;
}

void View::Record::setEditorName(const QString &editor_name)
{
    _editor_name = editor_name;
}

void View::Record::setFormats(const QString &formats)
{
    _formats = formats;
}

void View::Record::setEncoding(const QString &encoding)
{
    _encoding = encoding;
}

void View::Record::setHasIntelisense(bool has)
{
    _is_set_intellisense = true;
    _has_intellisense = has;
}

void View::Record::setHasPlugins(bool has)
{
    _is_set_plugins = true;
    _has_plugins = has;
}

void View::Record::setCanCompile(bool can)
{
    _is_set_compile = true;
    _can_compile = can;
}

QMap<QString, QString> View::Record::asXml() const
{
    QMap<QString,QString> map;
    map[editor_key] = _editor_name;
    map[formats_key] = _formats;//toString(_formats);
    map[encoding_key] = _encoding;//toString(_encoding);
    map[intellisense_key] = toString(_has_intellisense);
    map[plugins_key] = toString(_has_plugins);
    map[compile_key] = toString(_can_compile);
    return map;
}

bool View::Record::fromXml(const QMap<QString, QString> &map)
{
    _editor_name = map.value(editor_key,"");
    _formats = map.value(formats_key,"");
    _encoding = map.value(encoding_key,"");
    auto val = map.value(intellisense_key,"");
    if(val == true_str || val == false_str)
        setHasIntelisense(toBool(val));
    val = map.value(plugins_key,"");
    if(val == true_str || val == false_str)
        setHasPlugins(toBool(val));
    val = map.value(compile_key,"");
    if(val == true_str || val == false_str)
        setCanCompile(toBool(val));
    return true;
}

QJsonObject View::Record::asJson() const
{
    QJsonObject object;
    object[editor_key] = _editor_name;
    object[formats_key] = _formats;//QJsonArray::fromStringList(_formats);
    object[encoding_key] = _encoding;//QJsonArray::fromStringList(_encoding);
    object[intellisense_key] = _has_intellisense;
    object[plugins_key] = _has_plugins;
    object[compile_key] = _can_compile;
    return object;
}

bool View::Record::fromJson(const QJsonObject &object)
{
    _editor_name = object.value(editor_key).toString("");
    _formats = object.value(formats_key).toString("");//.toVariant().value<QStringList>();
    _encoding = object.value(encoding_key).toString("");//.toVariant().value<QStringList>();
    if(object.contains(intellisense_key)&& object.value(intellisense_key).isBool())
        setHasIntelisense(object.value(intellisense_key).toBool(false));
    if(object.contains(plugins_key) && object.value(plugins_key).isBool())
        setHasPlugins(object.value(plugins_key).toBool(false));
    if(object.contains(compile_key) && object.value(compile_key).isBool())
        setCanCompile(object.value(compile_key).toBool(false));
    return true;
}



















