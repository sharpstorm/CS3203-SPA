#pragma once

#include "QPSError.h"

const char QPS_PARSER_ERR_UNKNOWN_SYNONYM[] = "Unknown synonym encountered";
const char QPS_PARSER_ERR_SYNONYM_TYPE[] = "Invalid synonym type in clause";
const char QPS_PARSER_ERR_DUPLICATE_SYN[] = "Found duplicate synonym name";
const char QPS_PARSER_ERR_INVALID_STMT[] =
    "Statement ref provided is invalid";
const char QPS_PARSER_ERR_INVALID_WILDCARD[] =
    "Wildcard disallowed as argument";
const char QPS_PARSER_ERR_INVALID_ATTRIBUTE[] = "Invalid attribute in clause";

class QPSParserSemanticError : public QPSError {
 public:
  explicit QPSParserSemanticError(const ErrorMessage &message) :
      QPSError(message) {}
};
