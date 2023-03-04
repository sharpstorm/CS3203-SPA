#include <string>
#include <vector>
#include "SourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

using std::string, std::vector;

AST SourceParser::parseSource(string input) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(&input);
  return sourceTokenParser.parseProcedure(tokens.get());
}

AST SourceParser::parseExpression(string expression) {
  SourceTokenStreamPtr tokens = sourceLexer.tokenize(&expression);
  return sourceTokenParser.parseExpression(tokens.get());
}
