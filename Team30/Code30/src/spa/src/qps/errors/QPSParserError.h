#pragma once

#include <string>
#include <iostream>
#include "QPSError.h"

using std::string;

const char QPS_PARSER_ERR_EOS[] = "Unexpected end of token stream";
const char QPS_PARSER_ERR_UNEXPECTED[] =
    "Unexpected token encountered during parsing";
const char QPS_PARSER_ERR_UNKNOWN_TOKEN[] = "Unknown synonym encountered";
const char QPS_PARSER_ERR_PATTERN_TYPE[] = "Invalid synonym type in pattern";
const char QPS_PARSER_ERR_TOKEN_SEQUENCE[] = "Unexpected sequence of clauses";

class QPSParserError: public QPSError {
 public:
  explicit QPSParserError(string message) : QPSError(message) {}
};
