#include "records_table_model.h"

#include "algorithm"

#include <QDebug>

View::RecordsTableModel::RecordsTableModel(QObject *parent): QAbstractTableModel(parent)
{

}


void View::RecordsTableModel::append(const QPair<QString, Record> &record)
{
    beginInsertRows(QModelIndex(),_records.count(),_records.count());
    _records.append(record);
    endInsertRows();
}

void View::RecordsTableModel::setData(QList<QPair<QString, Record> > records)
{
    beginResetModel();
    _records.clear();
    _records = records;
    endResetModel();
}

void View::RecordsTableModel::clear()
{
    beginRemoveRows(QModelIndex(),0,_records.count()-1);
    _records.clear();
    endRemoveRows();
}

View::Record View::RecordsTableModel::getRecord(const QString &uuid) const
{
    auto pred = [uuid](const QPair<QString,Record>& pair){
        return   uuid == pair.first;
    };
    auto it = std::find_if(_records.cbegin(),_records.cend(),pred);
    if(it != _records.end())
    {
        return it->second;
    }
    return {};
}

int View::RecordsTableModel::rowCount(const QModelIndex &parent) const
{
    return _records.count();
}

int View::RecordsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 6;
}

QVariant View::RecordsTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case RecordsField::EditorName:
            value = _records.at(index.row()).second.editorName();
            break;
        case RecordsField::Formats:
            value = _records.at(index.row()).second.getFormats();
            break;
        case RecordsField::Encoding:
            value = _records.at(index.row()).second.getEncoding();
            break;
        case RecordsField::Intellisense:
            value = _records.at(index.row()).second.hasIntellisense();
            break;
        case RecordsField::Plugins:
            value = _records.at(index.row()).second.hasPlugins();
            break;
        case RecordsField::Compile:
            value = _records.at(index.row()).second.canCompile();
            break;
        default:
            break;
        }
        break;
    case Qt::EditRole:
        switch (index.column())
        {
        case RecordsField::EditorName:
            value = _records.at(index.row()).second.editorName();
            break;
        case RecordsField::Formats:
            value = _records.at(index.row()).second.getFormats();
            break;
        case RecordsField::Encoding:
            value = _records.at(index.row()).second.getEncoding();
            break;
        case RecordsField::Intellisense:
            value = _records.at(index.row()).second.hasIntellisense();
            break;
        case RecordsField::Plugins:
            value = _records.at(index.row()).second.hasPlugins();
            break;
        case RecordsField::Compile:
            value = _records.at(index.row()).second.canCompile();
            break;
        default:
            break;
        }
    default:
        break;
    }
    return value;
}

bool View::RecordsTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() >= _records.count() || index.column() > 5)
        return false;

    bool ok = false;
    switch (role)
    {
    case Qt::EditRole:
        switch (index.column())
        {
        case RecordsField::EditorName:
            _records[index.row()].second.setEditorName(value.toString());
            break;
        case RecordsField::Formats:
            _records[index.row()].second.setFormats(value.toString());
            break;
        case RecordsField::Encoding:
            _records[index.row()].second.setEncoding(value.toString());
            break;
        case RecordsField::Intellisense:
            _records[index.row()].second.setHasIntelisense(value.toBool());
            break;
        case RecordsField::Plugins:
            _records[index.row()].second.setHasPlugins(value.toBool());
            break;
        case RecordsField::Compile:
            _records[index.row()].second.setCanCompile(value.toBool());
            break;
        default:
            break;
        }
        ok = true;
        Q_EMIT dataChanged(index, index,
                           {Qt::DisplayRole, Qt::EditRole});
        recordUpdated(_records[index.row()].first);
    default:
        break;
    }
    return ok;
}

QVariant View::RecordsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant value;
    if(orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
        case RecordsField::EditorName:
            value = tr("Название");
            break;
        case RecordsField::Formats:
            value = tr("Форматы");
            break;
        case RecordsField::Encoding:
            value = tr("Кодировки");
            break;
        case RecordsField::Intellisense:
            value = "Intellisense";
            break;
        case RecordsField::Plugins:
            value = tr("Плагины");
            break;
        case RecordsField::Compile:
            value = tr("Компиляция");
        default:
            break;
        }
        return value;
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

Qt::ItemFlags View::RecordsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return flags | Qt::ItemIsEditable;
}
