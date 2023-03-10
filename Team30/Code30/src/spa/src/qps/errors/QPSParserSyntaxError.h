#pragma once

#include "QPSError.h"
#include <string>

using std::string;

const char QPS_PARSER_ERR_EOS[] = "Unexpected end of token stream";
const char QPS_PARSER_ERR_UNEXPECTED[] =
    "Unexpected token encountered during parsing";
const char QPS_PARSER_ERR_TOKEN_SEQUENCE[] = "Unexpected sequence of clauses";
const char QPS_PARSER_ERR_PATTERN_TYPE[] = "Invalid synonym type in pattern";
const char QPS_PARSER_ERR_WITH_TYPE[] = "Invalid attribute in with";
const char QPS_PARSER_ERR_NO_SELECT[] = "No select clause";
const char QPS_PARSER_ERR_INVALID_PATTERN[] = "Invalid Pattern String";

class QPSParserSyntaxError: public QPSError {
 public:
  explicit QPSParserSyntaxError(string message) : QPSError(message) {}
};
