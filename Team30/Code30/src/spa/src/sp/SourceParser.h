#pragma once

#include "ISourceParser.h"
#include "sp/parser/SourceTokenParser.h"
#include "sp/lexer/SourceLexerFactory.h"

class SourceParser : public ISourceParser {
 public:
  ASTPtr parseSource(const FileData &input) override;
  ASTPtr parseExpression(const SourceExpression &expression) override;

 private:
  SourceLexerFactory lexerFactory;
  SourceTokenParser sourceTokenParser;
};
