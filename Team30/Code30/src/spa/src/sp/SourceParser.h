#pragma once

#include <string>
#include "ISourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

using std::string;

class SourceParser : public ISourceParser {
 public:
  AST parseSource(string input);
  AST parseExpression(string expression);

 private:
  SourceLexer sourceLexer;
  SourceTokenParser sourceTokenParser;
};

