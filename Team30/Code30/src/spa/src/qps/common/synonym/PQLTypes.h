#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

using std::string, std::unordered_set, std::vector, std::unique_ptr;

enum PQLSynonymType {
  PQL_SYN_TYPE_VARIABLE,
  PQL_SYN_TYPE_CONSTANT,
  PQL_SYN_TYPE_PROCEDURE,
  PQL_SYN_TYPE_STMT = 0x100,
  PQL_SYN_TYPE_READ,
  PQL_SYN_TYPE_PRINT,
  PQL_SYN_TYPE_CALL,
  PQL_SYN_TYPE_WHILE,
  PQL_SYN_TYPE_IF,
  PQL_SYN_TYPE_ASSIGN
};

enum AttributeReturnMask {
  NO_RETURN_MASK = 0x0,
  STR_RETURN_MASK = 0x10,
  INT_RETURN_MASK = 0x20
};

enum PQLSynonymAttribute {
  NO_ATTRIBUTE = NO_RETURN_MASK,
  PROC_NAME = STR_RETURN_MASK,
  VAR_NAME,
  CONST_VALUE = INT_RETURN_MASK,
  STMT_NUM
};

const int PQL_SYN_STMT_MASK = PQL_SYN_TYPE_STMT;
typedef string PQLSynonymName;
typedef unordered_set<PQLSynonymName> PQLSynonymNameSet;
typedef vector<PQLSynonymName> PQLSynonymNameList;
typedef unique_ptr<PQLSynonymNameList> PQLSynonymNameListPtr;
