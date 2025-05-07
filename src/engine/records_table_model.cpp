#include "records_table_model.h"

#include <QDebug>

#include "database/table.h"

View::RecordsTableModel::RecordsTableModel(std::shared_ptr<Table> table, QObject *parent): QAbstractTableModel(parent), _table(table)
{

}

void View::RecordsTableModel::loadData()
{
    beginResetModel();
    _records.clear();
    auto ids = _table->IDs();
    for(const auto& id : ids)
    {
        _records.append({id,_table->getRecord(id)});
    }
    endResetModel();
}

// void View::RecordsTableModel::appendData(const QList<QPair<QString, Record> > &records)
// {
//     beginInsertRows(QModelIndex(),_records.count(),_records.count()+records.count()-1);
//     _records.append(records);
//     endInsertRows();
// }

void View::RecordsTableModel::append(const QPair<QString, Record> &record)
{
    beginInsertRows(QModelIndex(),_records.count(),_records.count());
    _records.append(record);
    endInsertRows();
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
    }
    return value;
}
