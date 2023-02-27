#pragma once

#include "QPSError.h"
#include <string>

using std::string;

const char QPS_LEXER_ERR_INTEGER_ZERO[] = "Integer token starts with zero";
const char QPS_LEXER_ERR_STRING_DIGIT[] = "String token starts with digit";
const char QPS_LEXER_ERR_INVALID_CHAR[] = "Detected invalid character";
const char QPS_LEXER_ERR_EMPTY_QUOTE[] = "Empty quoted content";
const char QPS_LEXER_ERR_UNMATCHED_QUOTE[] = "Unmatched quote";

class QPSLexerError: public QPSError {
 public:
  explicit QPSLexerError(string message) : QPSError(message) {}
};
