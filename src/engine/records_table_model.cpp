#include "records_table_model.h"

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
    default:
        break;
    }
    return value;
}

bool View::RecordsTableModel::setData(const QModelIndex &/*index*/, const QVariant &/*value*/, int /*role*/)
{
    return true;
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
