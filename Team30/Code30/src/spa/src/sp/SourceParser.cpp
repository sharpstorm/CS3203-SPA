#include <vector>

#include "SourceParser.h"

using std::vector;

ASTPtr SourceParser::parseSource(const FileData &input) {
  SourceTokenStreamPtr tokens = lexerFactory.makeLexer(&input).tokenize();
  return sourceTokenParser.parseProgram(tokens.get());
}

ASTPtr SourceParser::parseExpression(const SourceExpression &expression) {
  SourceTokenStreamPtr tokens = lexerFactory.makeLexer(&expression).tokenize();
  return sourceTokenParser.parseExpression(tokens.get());
}
