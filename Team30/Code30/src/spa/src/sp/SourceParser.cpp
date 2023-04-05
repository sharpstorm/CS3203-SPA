#include <vector>

#include "SourceParser.h"

using std::vector;

ASTPtr SourceParser::parseSource(const FileData &input) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(input);
  return sourceTokenParser.parseProgram(tokens.get());
}

ASTPtr SourceParser::parseExpression(const SourceExpression &expression) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(expression);
  return sourceTokenParser.parseExpression(tokens.get());
}
