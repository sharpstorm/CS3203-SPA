#include <string>
#include <vector>
#include <utility>

#include "SourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

using std::string, std::vector;

ASTPtr SourceParser::parseSource(string input) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(&input);
  return std::move(sourceTokenParser.parseProgram(tokens.get()));
}

ASTPtr SourceParser::parseExpression(string expression) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(&expression);
  return std::move(sourceTokenParser.parseExpression(tokens.get()));
}
