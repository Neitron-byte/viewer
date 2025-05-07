#pragma once
#include <functional>

#include <QHash>

namespace View
{
class IFileReader;

/*!
 * \brief The ReaderFactory class
 * Фабрика читателец
 * читатель определяется по расширению файла
 */

class ReaderFactory
{
public:
    ReaderFactory() = default;
    ~ReaderFactory() = default;

    using readerCreator = std::function<IFileReader*()>;

    void registerCreator(const QString& extension, readerCreator creator);

    IFileReader* createReader(const QString& extension);

private:
    QHash<QString,readerCreator> _creators;
};
}
