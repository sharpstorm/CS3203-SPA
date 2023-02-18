#include <vector>

#include "../../../../spa/src/sp/parser/SourceTokenParser.h"
#include "common/AST.h"

using std::vector;

class SourceTokenParserStub : public SourceTokenParser {
 public:
  SourceTokenParserStub() {}

  SourceParseState parseAndGetState(vector<SourceToken>* tokens) {
    SourceParseState state(tokens);
    return state;
  }
};
