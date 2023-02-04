#pragma once

#include <vector>
#include <memory>
#include "../common/SourceToken.h"
#include "GrammarContextProvider.h"

using std::shared_ptr;

class SourceTokenParser {
 public:
  SourceTokenParser();
  shared_ptr<ASTNode> parse(vector<SourceToken>* tokens);
};
