#pragma once

#include "QPSError.h"

const char QPS_PARSER_ERR_EOS[] = "Unexpected end of token stream";
const char QPS_PARSER_ERR_UNEXPECTED[] =
    "Unexpected token encountered during parsing";
const char QPS_PARSER_ERR_PATTERN_TYPE[] = "Invalid synonym type in pattern";
const char QPS_PARSER_ERR_WITH_TYPE[] = "Invalid attribute in with";
const char QPS_PARSER_ERR_INVALID_PATTERN[] = "Invalid Pattern String";

class QPSParserSyntaxError : public QPSError {
 public:
  explicit QPSParserSyntaxError(const ErrorMessage &message) :
      QPSError(message) {}
};
