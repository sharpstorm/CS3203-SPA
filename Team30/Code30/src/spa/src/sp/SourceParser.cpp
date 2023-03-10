#include <string>
#include <vector>

#include "SourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

using std::string, std::vector;

ASTPtr SourceParser::parseSource(string input) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(&input);
  return sourceTokenParser.parseProgram(tokens.get());
}

ASTPtr SourceParser::parseExpression(string expression) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(&expression);
  return sourceTokenParser.parseExpression(tokens.get());
}
