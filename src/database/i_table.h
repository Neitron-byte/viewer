#pragma once
#include "data/record.h"

namespace View
{
/*!
 * \brief The ITable class
 * Интерфейсный класс работы с таблицей БД
 */
class ITable
{
public:
    virtual ~ITable() = default;

    virtual bool create() const = 0; // создание таблицы
    virtual bool drop() const = 0;   // удаление таблицы

    virtual bool append(const QString &id, const Record &rec) = 0;//добавление записи(строки) в таблицу
    virtual bool update(const QString &id, const Record &rec) = 0;//обновление записи
    virtual bool remove(const QString& id) = 0;//удаление записи из таблицы
    virtual bool contains(const QString& id) const = 0;//проверка на содержание записи
    virtual Record getRecord(const QString& id) const = 0;//получение записи по ID
    virtual QList<QString> IDs() const = 0;//все ID    

    virtual int count() const = 0;//количество записей в таблице

    virtual QString error() const = 0;

    virtual bool clear() = 0;
};
}
