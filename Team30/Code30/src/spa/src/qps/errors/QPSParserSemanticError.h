#pragma once

#include "QPSError.h"

const char QPS_PARSER_ERR_UNKNOWN_SYNONYM[] = "Unknown synonym encountered";
const char QPS_PARSER_ERR_SYNONYM_TYPE[] = "Invalid synonym type in clause";
const char QPS_PARSER_ERR_DUPLICATE_SYN[] = "Found duplicate synonym name";
const char QPS_PARSER_ERR_UNKNOWN_SELECT[] =
    "Select clause references unknown synonym";

class QPSParserSemanticError: public QPSError {
 public:
  explicit QPSParserSemanticError(const char* message) : QPSError(message) {}
};
