#include <string>
#include <vector>

#include "../../../../spa/src/sp/SourceParser.h"
#include "../../../../spa/src/sp/parser/SourceParseState.h"
#include "sp/lexer/SourceLexer.h"

class SourceParserStub : public SourceParser {
 public:
  SourceParseState parseTokens_AndGetState(vector<SourceToken> tokens) {
    SourceParseState state(&tokens);
    return state;
  }
};
