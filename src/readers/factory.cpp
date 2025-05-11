// #include "factory.h"

// #include "i_file_reader.h"

// void View::ReaderFactory::registerCreator(const QString &extension, readerCreator creator)
// {
//     _creators[extension] = creator;
// }

// std::unique_ptr<View::IFileReader> View::ReaderFactory::createReader(const QString &extension)
// {
//     auto it = _creators.find(extension);
//     if(it != _creators.end())
//     {
//         return it.value()();
//     }
//     return nullptr;
// }
