#include <string>
#include <vector>
#include "SourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

using std::string, std::vector;

AST SourceParser::parseSource(string input) {
  SourceLexer sourceLexer;
  SourceTokenParser sourceTokenParser;
  vector<SourceToken> tokens = sourceLexer.tokenize(&input);
  return sourceTokenParser.parse(&tokens);
}
