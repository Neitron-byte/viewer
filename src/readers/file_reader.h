#pragma once

#include <types.h>

namespace View
{
class FileReader
{
public:
    FileReader() = default;
    ~FileReader() = default;

    bool read(const QString& path_dir, QList<Record>& records);

};
}
