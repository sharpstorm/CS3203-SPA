#pragma once

#include <vector>
#include "../common/SourceToken.h"
#include "GrammarContextProvider.h"
#include "common/ast/AST.h"

class SourceTokenParser {
 public:
  SourceTokenParser() = default;

  template <SourceGrammarContextType STARTING_CONTEXT>
  AST parse(vector<SourceToken>* tokens);

 private:
  GrammarContextProvider gcp;
};

template <SourceGrammarContextType STARTING_CONTEXT>
AST SourceTokenParser::parse(vector<SourceToken>* tokens) {
  SourceParseState state(tokens);
  gcp.getContext(STARTING_CONTEXT)->generateSubtree(&state);
  return AST(state.getCached());
}
