#pragma once
#include <memory>

#include <QAbstractTableModel>
#include <QObject>

#include "data/record.h"

namespace View
{
class Table;

/*!
 * \brief The RecordsTableModel class
 * Табличная модель записей
 * Хранить записи и уникальные идентификаторы к ним
 */
class RecordsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum RecordsField
    {
        EditorName = 0,
        Formats,
        Encoding,
        Intellisense,
        Plugins,
        Compile
    };

    explicit RecordsTableModel(std::shared_ptr<Table> table, QObject* parent = nullptr);


    void loadData();

    void appendData(const QList<QPair<QString,Record>>& records);


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::shared_ptr<Table> _table;

    QList<QPair<QString,Record>> _records;
};
}
