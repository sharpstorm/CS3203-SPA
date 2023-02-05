#include <vector>
#include "SourceTokenParser.h"
#include "sp/common/AST.h"

using std::vector;

SourceTokenParser::SourceTokenParser() {}

AST SourceTokenParser::parse(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  GrammarContextProvider gcp;
  gcp.getContext(PROCEDURE_CONTEXT)->generateSubtree(&state);
  return AST(state.getCached());
}
