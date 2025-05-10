#include "table_wrapper.h"

View::TableWrapper::TableWrapper(std::unique_ptr<Table> table, QObject *parent): QObject(parent)
    , _table{std::move(table)}
{

}

void View::TableWrapper::create(boolHandler callback)
{
    executeAsync<bool>([this](){
        return _table->create();
    },callback);
}

void View::TableWrapper::drop(boolHandler callback)
{
    executeAsync<bool>([this](){
        return _table->drop();
    },callback);
}

void View::TableWrapper::update(const QString &id, const Record &rec, boolHandler callback)
{
    executeAsync<bool>([this,id,rec](){
        return _table->update(id,rec);
    },callback);
}

void View::TableWrapper::remove(const QString &id, boolHandler callback)
{
    executeAsync<bool>([this,id](){
        return _table->remove(id);
    },callback);
}

void View::TableWrapper::contains(const QString &id, boolHandler callback) const
{
    executeAsync<bool>([this,id](){
        return _table->contains(id);
    },callback);
}

void View::TableWrapper::getRecord(const QString &id, recordHandler callback) const
{
    executeAsync<Record>([this,id](){
        return _table->getRecord(id);
    },callback);
}

void View::TableWrapper::IDs(listHandler callback) const
{
    executeAsync<QList<QString>>([this](){
        return _table->IDs();
    },callback);
}

void View::TableWrapper::count(countHandler callback) const
{
    executeAsync<int>([this](){
        return _table->count();
    },callback);
}

QString View::TableWrapper::error() const
{
    return _table->error();
}

void View::TableWrapper::clear(boolHandler callback)
{
    executeAsync<bool>([this](){
        return _table->clear();
    },callback);
}

bool View::TableWrapper::isConnected() const
{
    return _table->isConnected();
}
