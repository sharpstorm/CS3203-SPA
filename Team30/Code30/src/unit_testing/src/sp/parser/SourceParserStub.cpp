#include <string>
#include <vector>

#include "../../../../spa/src/sp/parser/SourceParseState.h"
#include "SourceParser.h"
#include "sp/lexer/SourceLexer.h"
#include "sp/parser/SourceTokenParser.h"

class SourceParserStub : public SourceParser {
  SourceParseState parseSource_AndGetState(string input) {
    SourceLexer sourceLexer;
    SourceTokenParserStub stub;
    vector<SourceToken> tokens = sourceLexer.tokenize(&input);
    return stub.parseAndGetState(&tokens);
  }
};