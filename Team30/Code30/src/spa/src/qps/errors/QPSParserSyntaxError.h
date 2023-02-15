#pragma once

#include "QPSError.h"

const char QPS_PARSER_ERR_EOS[] = "Unexpected end of token stream";
const char QPS_PARSER_ERR_UNEXPECTED[] =
    "Unexpected token encountered during parsing";
const char QPS_PARSER_ERR_TOKEN_SEQUENCE[] = "Unexpected sequence of clauses";

class QPSParserSyntaxError: public QPSError {
 public:
  explicit QPSParserSyntaxError(const char* message) : QPSError(message) {}
};
