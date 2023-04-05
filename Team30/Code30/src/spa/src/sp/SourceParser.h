#pragma once

#include "ISourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

class SourceParser : public ISourceParser {
 public:
  ASTPtr parseSource(const FileData &input) override;
  ASTPtr parseExpression(const SourceExpression &expression) override;

 private:
  SourceLexer sourceLexer;
  SourceTokenParser sourceTokenParser;
};
