#include "SourceTokenParser.h"
#include <iostream>

SourceTokenParser::SourceTokenParser() {
};

shared_ptr<ASTNode> SourceTokenParser::parse(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  GrammarContextProvider gcp;
  gcp.getContext(REL_CONTEXT)->generateSubtree(&state);

  return state.getCached();
}
