#pragma once

#include <string>
#include <memory>

using std::string, std::unique_ptr;

typedef string FilePath;
typedef string FileData;

typedef unique_ptr<FileData> FileDataPtr;

typedef string SourceExpression;
