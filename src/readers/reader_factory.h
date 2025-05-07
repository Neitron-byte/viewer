#pragma once
#include <memory>
#include <functional>

#include <QHash>
#include <QString>

namespace View
{
class IFileReader;

/*!
 * \brief The ReaderFactory class
 * Фабрика читателей
 * читатель определяется по расширению файла
 */

class ReaderFactory
{
public:
    ReaderFactory() = default;
    ~ReaderFactory() = default;

    using readerCreator = std::function<std::unique_ptr<IFileReader>()>;

    void registerCreator(const QString& extension, readerCreator creator);

    std::unique_ptr<IFileReader> createReader(const QString& extension);

private:
    QHash<QString,readerCreator> _creators;
};
}
