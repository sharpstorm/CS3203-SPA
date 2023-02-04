#include "SourceTokenParser.h"
#include <iostream>

SourceTokenParser::SourceTokenParser() {
};

shared_ptr<ASTNode> SourceTokenParser::parse(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  GrammarContextProvider gcp;
  return gcp.getContext(VARIABLE_CONTEXT)->generateSubtree(&state);

//  return state.getCached();
}
