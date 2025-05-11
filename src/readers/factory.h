#pragma once
#include <memory>
#include <functional>

#include <QHash>
#include <QString>

#include "i_file_reader.h"
#include "i_file_writer.h"

namespace View
{
/*!
 * \brief The Factory class
 * Фабрика читателей / писателей
 * читатель определяется по расширению файла
 */
template<typename T>
class Factory
{
public:
    Factory() = default;
    ~Factory() = default;

    using creator = std::function<std::unique_ptr<T>()>;

    void registerCreator(const QString& extension, creator creator);

    std::unique_ptr<T> create(const QString& extension);

private:
    QHash<QString,creator> _creators;
};

template<typename T>
inline void Factory<T>::registerCreator(const QString &extension, creator creator)
{
    _creators[extension] = creator;
}

template<typename T>
inline std::unique_ptr<T> Factory<T>::create(const QString &extension)
{
    auto it = _creators.find(extension);
    if(it != _creators.end())
    {
        return it.value()();
    }
    return nullptr;
}


using ReaderFactory = Factory<IFileReader>;
using WriterFactory = Factory<IFileWriter>;

}
