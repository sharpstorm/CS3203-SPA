#pragma once

#include <string>
#include "ISourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

using std::string;

class SourceParser : public ISourceParser {
 public:
  ASTPtr parseSource(string input) override;
  ASTPtr parseExpression(string expression) override;

 private:
  SourceLexer sourceLexer;
  SourceTokenParser sourceTokenParser;
};

