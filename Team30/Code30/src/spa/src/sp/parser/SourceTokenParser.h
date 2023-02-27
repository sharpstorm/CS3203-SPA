#pragma once

#include <vector>
#include "../common/SourceToken.h"
#include "GrammarContextProvider.h"
#include "common/ast/AST.h"

class SourceTokenParser {
 public:
  SourceTokenParser();
  AST parse(vector<SourceToken>* tokens);

 private:
  GrammarContextProvider gcp;
};
