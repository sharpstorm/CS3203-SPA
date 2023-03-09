#pragma once

#include <string>

using std::string;

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

enum PQLSynonymAttribute {
  NO_ATTRIBUTE,
  PROC_NAME,
  VAR_NAME,
  VALUE,
  STMT_NUM
};

const int PQL_SYN_STMT_MASK = PQL_SYN_TYPE_STMT;
typedef string PQLSynonymName;
