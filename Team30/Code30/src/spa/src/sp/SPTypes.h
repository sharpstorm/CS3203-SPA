#pragma once

#include <string>
#include <memory>
#include <unordered_set>

using std::string, std::unique_ptr, std::unordered_set;

typedef string FilePath;
typedef string FileData;

typedef unique_ptr<FileData> FileDataPtr;

typedef string SourceExpression;
typedef int LineNumber;
typedef string ProcedureName;
typedef string VariableName;
typedef unordered_set<VariableName> VariableNameSet;
